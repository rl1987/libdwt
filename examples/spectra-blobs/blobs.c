/**
 * @brief Find strongest ridges of the S transform plane.
 */

#include "libdwt.h"
#include <stddef.h> // size_t
#include "image.h" // image_t, ...
#include <complex.h> // complex
#include "gabor.h" // gaussian_center
#include "system.h" // dwt_util_free
#include "util.h" // dwt_util_find_max_pos_s
#include <assert.h> // assert
#include <math.h> // sqrtf

void remove_ridge(
	image_t *ridges,
	image_t *maxima
)
{
	// for each y
	for(int y = 0; y < maxima->size_y; y++)
		// for each x
		for(int x = 0; x < maxima->size_x; x++)
			// when the point is a maximum
			if( *image_coeff_s(maxima, y, x) > 0.f )
				// remove the ridge
#if 0
				*image_coeff_s(ridges, y, x) = 0.f;
#else
				*image_coeff_s(ridges, y, x) *= 1.f - *image_coeff_s(maxima, y, x);
#endif
}

static
float s_sigma(float f)
{
	assert( f != 0.f );

	const float alpha = f * f;
	const float sigma = sqrtf(1.f/2.f/alpha);

	return sigma;
}

// extern
void s_gen_kernel(
	float complex **ckern,
	int stride,
	float f
);

int get_maximum(
	image_t *ridges,
	image_t *maxima,
	int *pos_x,
	int *pos_y
)
{
	// find maximum
	dwt_util_find_max_pos_s(
		// input
		ridges->ptr,
		ridges->size_x,
		ridges->size_y,
		ridges->stride_x,
		ridges->stride_y,
		// output
		pos_x,
		pos_y
	);

#if 1
	dwt_util_log(LOG_DBG, "maximum found at (%i,%i)\n", *pos_y, *pos_x);
#endif

	// add the surrounding pixels
	{
		float complex *ckern = 0;
		int ckern_stride = sizeof(float complex);

		// for each row
		for(int y = 0; y < maxima->size_y; y++)
		{
			float norm1_y = (y+1.f)/(float)maxima->size_y;
			float f = norm1_y * 0.5f;
			float a = 1.f;
			float sigma = s_sigma(f);
			int center = gaussian_center(sigma, a);
			int size = gaussian_size(sigma, a);

			s_gen_kernel(&ckern, ckern_stride, f);
			float kern[size];
			dwt_util_vec_cabs_cs(kern, sizeof(float), ckern, ckern_stride, size);

			float norm_factor = 1.f/kern[center];

			for(int x = *pos_x-center; x < *pos_x-center+size; x++)
			{
				if( x < 0 || x >= maxima->size_x )
					continue;

				*dwt_util_addr_coeff_s(
					maxima->ptr,
					maxima->size_y-y-1,
					x,
					maxima->stride_x,
					maxima->stride_y
				) =
#if 0
				1.f;
#else
				kern[center+x-*pos_x] * norm_factor;
#endif
			}
		}

		dwt_util_free(ckern);
	}

	remove_ridge(ridges, maxima);

	return 1;
}

void spectra_st_get_strongest_ridges(
	image_t *plane,	// input
	image_t *points,	// output
	int ridges_no		// number of the strongest points
)
{
	// alloc "ridges"
	struct image_t ridges = (image_t){
		.ptr = dwt_util_alloc_image2(plane->stride_x, plane->stride_y, plane->size_x, plane->size_y),
		.size_x = plane->size_x,
		.size_y = plane->size_y,
		.stride_x = plane->stride_x,
		.stride_y = plane->stride_y
	};

#if 1
	// extract ridges
	detect_ridges1_s(
		plane->ptr,
		ridges.ptr,
		plane->stride_x,
		plane->stride_y,
		plane->size_x,
		plane->size_y,
		0.f
	);
#else
	ridges.ptr = plane->ptr;
#endif

	//  alloc "maxima"
	struct image_t maxima = (image_t){
		.ptr = dwt_util_alloc_image2(plane->stride_x, plane->stride_y, plane->size_x, plane->size_y),
		.size_x = plane->size_x,
		.size_y = plane->size_y,
		.stride_x = plane->stride_x,
		.stride_y = plane->stride_y
	};

	// strongest ridges
	for(int i = 0; i < ridges_no; i++)
	{
		image_zero(&maxima);

		int pos_x, pos_y;

		get_maximum(&ridges, &maxima, &pos_x, &pos_y);

		*image_coeff_s(points, i, 0) = pos_x;
		*image_coeff_s(points, i, 1) = pos_y;

#if 0
		// store "maxima" and "ridges"
		image_save_to_pgm_format_s(&maxima, "maxima-%i.pgm", i);
		image_save_to_pgm_format_s(&ridges, "ridges-%i.pgm", i);
#endif
	}

	image_free(&maxima);
	image_free(&ridges);
}

int main(int argc, char *argv[])
{
	dwt_util_init();

	const char *path = (argc > 1) ? argv[1] :
		"data/st-log/st-sum-class-all.mat";

	image_t *plane = image_create_from_mat_s(path);

	if( !plane )
		dwt_util_error("unable to load from %s\n", path);

	dwt_util_log(LOG_INFO, "plane size=(%i,%i)\n", plane->size_y, plane->size_x);

	image_save_to_pgm_s(plane, "plane.pgm");

	const int ridges_no = 20;

	// strongest points: size_x=2, size_y=ridges_no
	image_t *points = image_create_s(2, ridges_no);

	// strongest ridges
	spectra_st_get_strongest_ridges(plane, points, ridges_no);

	image_save_to_mat_s(points, "points.mat");

	image_destroy(points);

	image_destroy(plane);

	dwt_util_finish();

	return 0;
}
