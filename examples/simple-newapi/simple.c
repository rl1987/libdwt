/**
 * @file
 * @author David Barina <ibarina@fit.vutbr.cz>
 * @brief Simple example application showing usage of libdwt functions.
 */

#include "libdwt.h"
#include "dwt-simple.h"

int main()
{
	// init platform
	dwt_util_init();

	// set threads, workes, accel_type
	dwt_util_set_num_threads(1);
	dwt_util_set_num_workers(1);
	dwt_util_set_accel(0);

	// image size
	for(int size=512; size<=513; size++)
	{
		dwt_util_log(LOG_INFO, "size=%i\n", size);

		const int x = size, y = size;

		// compute optimal stride
		const int stride_y = sizeof(float);
		const int stride_x = dwt_util_get_opt_stride(stride_y * x);

		// image data
		void *data1, *data2, *data3;

		// decomposition
		int j = -1;

		dwt_util_log(LOG_INFO, "generating test images...\n");

		// create test images
		dwt_util_alloc_image(&data1, stride_x, stride_y, x, y);
		dwt_util_test_image_fill_s(data1, stride_x, stride_y, x, y, 0);
		dwt_util_alloc_image(&data2, stride_x, stride_y, x, y);
		dwt_util_copy_s(data1, data2, stride_x, stride_y, x, y);
		dwt_util_alloc_image(&data3, stride_x, stride_y, x, y);

		// init timer
		dwt_clock_t time_start;
		dwt_clock_t time_stop;
		const int type = dwt_util_clock_autoselect();

		dwt_util_log(LOG_INFO, "forward transform...\n");

		// start timer
		time_start = dwt_util_get_clock(type);

		// forward transform
		//dwt_cdf97_2f_inplace_s(data1, stride_x, stride_y, x, y, x, y, &j, 1, 0);
		//dwt_cdf97_2f_inplace_sdl_s(data1, stride_x, stride_y, x, y, x, y, &j, 1, 0);
		fdwt2_cdf97_diagonal_s(
		//fdwt2_cdf97_vertical_s(
		//fdwt2_cdf97_horizontal_s(
			data1,
			x,
			y,
			stride_x,
			stride_y,
			&j,
			0
		);

		// stop timer
		time_stop = dwt_util_get_clock(type);
		dwt_util_log(LOG_INFO, "elapsed time: %f secs\n", (double)(time_stop - time_start) / dwt_util_get_frequency(type));

		// convert transform into viewable format
		dwt_util_conv_show_s(data1, data3, stride_x, stride_y, x, y);

		dwt_util_log(LOG_INFO, "inverse transform...\n");

		// start timer
		time_start = dwt_util_get_clock(type);

		// inverse transform
		dwt_cdf97_2i_inplace_s(data1, stride_x, stride_y, x, y, x, y, j, 1, 0);

		// stop timer
		time_stop = dwt_util_get_clock(type);
		dwt_util_log(LOG_INFO, "elapsed time: %f secs\n", (double)(time_stop - time_start) / dwt_util_get_frequency(type));

		// compare
		if( dwt_util_compare_s(data1, data2, stride_x, stride_y, x, y) )
			dwt_util_log(LOG_INFO, "images differs\n");
		else
			dwt_util_log(LOG_INFO, "success\n");
		
		// release platform resources
		dwt_util_finish();

		// free allocated memory
		dwt_util_free_image(&data1);
		dwt_util_free_image(&data2);
		dwt_util_free_image(&data3);
	}

	return 0;
}
