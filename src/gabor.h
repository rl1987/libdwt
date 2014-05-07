#ifndef GABOR
#define GABOR

#include <complex.h>

/**
 * @brief Gabor function parametrized by the time, the width given by standard deviation (sigma) and the frequency.
 */
float complex gabor_function(
	float t,	///< time, centered at 0
	float sigma,	///< standard deviation
	float f		///< frequency in radians
);

/**
 * @brief Gabor wavelet parametrized by the scale, the time, the width given by standard deviation (sigma) and the frequency.
 */
float complex gabor_wavelet(
	float t,	///< time, centered at 0
	float sigma,	///< standard deviation
	float f,	///< frequency in radians
	float a		///< scale
);

/**
 * @brief Gets an effective support of the Gaussian function.
 */
int gaussian_size(
	float sigma,
	float a
);

/**
 * @brief Gets an index of the center of the Gaussian function.
 */
int gaussian_center(
	float sigma,
	float a
);

/**
 * @brief Dot product of a real signal and a complex kernel.
 */
float complex dwt_util_cdot1_s(
	const float *func,		///< the real signal
	int func_size,			///< size of the signal
	int func_stride,		///< stride of the signal elements
	int func_center,		///< displacement of the signal center
	const float complex *kern,	///< the complex kernel
	int kern_size,			///< size of the kernel
	int kern_stride,		///< stride of the kernel elements
	int kern_center			///< displacement of the kernel center
);

/**
 * @brief Calculate one line of a time-frequency plane (the magnitude).
 */
void timefreq_line(
	float *dst,			///< the line in TF plane
	int dst_stride,			///< stride of the TF-plane
	const float *src,		///< the analysed signal
	int src_stride,			///< stride of the signal
	int size,			///< size of both -- the signal and the TF-plane line
	const float complex *kern,	///< complex kernel
	int kern_stride,		///< stride of the kernel
	int kern_size,			///< size of the kernel
	int kern_center			///< center of the kernel
);

/**
 * @brief Calculate one line of a time-frequency plane (the complex angle).
 */
void timefreq_arg_line(
	float *dst,			///< the line in TF plane
	int dst_stride,			///< stride of the TF-plane
	const float *src,		///< the analysed signal
	int src_stride,			///< stride of the signal
	int size,			///< size of both -- the signal and the TF-plane line
	const float complex *kern,	///< complex kernel
	int kern_stride,		///< stride of the kernel
	int kern_size,			///< size of the kernel
	int kern_center			///< center of the kernel
);

/**
 * @brief Get a real component of the complex signal.
 */
void dwt_util_vec_creal_cs(
	float *dst,			///< put the real-valued signal here
	int dst_stride,			///< the input signal stride
	const float complex *src,	///< the complex-valued input signal
	int src_stride,			///< the output signal stride
	int size			///< the signal size
);

/**
 * @brief Generate the Gabor atom.
 */
void gabor_gen_kernel(
	float complex **ckern,		///< put the atom here
	int stride,			///< the stride of the output
	float sigma,			///< the std. deviation of the Gaussian envelope
	float freq,			///< the frequency in radians
	float a				///< the scale (default 1.0f)
);

/**
 * @brief Generate a real-valued test signal.
 */
void test_signal(
	float **dest,		///< put the result here
	int stride,		///< the stride of the output
	int size,		///< the requested signal size
	int type		///< a type of the test signal
);

/**
 * @brief Short-time Fourier transform using the Gaussian window (the complex magnitude).
 */
void gabor_ft_s(
	// input
	const float *sig,	///< the analysed signal
	int sig_stride,		///< the stride of the signal
	int sig_size,		///< the length of the signal, the width of the plane
	// output
	void *plane,		///< put the plane here
	int stride_x,		///< stride of rows of the plane
	int stride_y,		///< stride of columns of the plane
	int bins,		///< the height of the plane
	// params
	float sigma		///< std. deviation of the baseline kernel (implies the window size)
);

/**
 * @brief Continuous wavelet transform using the complex Morlet wavelet (the complex magnitude).
 */
void gabor_wt_s(
	// input
	const float *sig,	///< the analysed signal
	int sig_stride,		///< the stride of the signal
	int sig_size,		///< the length of the signal, the width of the plane
	// output
	void *plane,		///< put the plane here
	int stride_x,		///< stride of rows of the plane
	int stride_y,		///< stride of columns of the plane
	int bins,		///< the height of the plane
	// params
	float sigma,		///< std. deviation of the baseline kernel (implies the window size)
	float freq,		///< frequency of the baseline kernel
	float scale_factor	///< growing factor of the scale
);

/**
 * @brief S transform (the complex magnitude).
 */
void gabor_st_s(
	// input
	const float *sig,	///< the analysed signal
	int sig_stride,		///< the stride of the signal
	int sig_size,		///< the length of the signal, the width of the plane
	// output
	void *plane,		///< put the plane here
	int stride_x,		///< stride of rows of the plane
	int stride_y,		///< stride of columns of the plane
	int bins		///< the height of the plane
	// no params
);

/**
 * @brief S transform (the complex argument).
 */
void gabor_st_arg_s(
	// input
	const float *sig,	///< the analysed signal
	int sig_stride,		///< the stride of the signal
	int sig_size,		///< the length of the signal, the width of the plane
	// output
	void *plane,		///< put the plane here
	int stride_x,		///< stride of rows of the plane
	int stride_y,		///< stride of columns of the plane
	int bins		///< the height of the plane
	// no params
);

/**
 * @brief Angle (the complex argument) of continuous wavelet transform using the complex Morlet (Gabor) wavelet.
 */
void gabor_wt_arg_s(
	// input
	const float *sig,	///< the analysed signal
	int sig_stride,		///< the stride of the signal
	int sig_size,		///< the length of the signal, the width of the plane
	// output
	void *plane,		///< put the plane here
	int stride_x,		///< stride of rows of the plane
	int stride_y,		///< stride of columns of the plane
	int bins,		///< the height of the plane
	// params
	float sigma,		///< std. deviation of the baseline kernel (implies the window size)
	float freq,		///< frequency of the baseline kernel
	float scale_factor	///< growing factor of the scale
);

#endif
