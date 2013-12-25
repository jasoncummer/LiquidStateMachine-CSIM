/** \file gaussiananalogfilter.cpp
**  \brief Analog gaussian low-pass filter.
*/

#include "gaussiananalogfilter.h"
#include <math.h>


/** Constructs a new gaussian analog filter.
    \param kernel_length The length of the filter kernel.
    \param std_dev Standard deviation of the filter mask. */
GaussianAnalogFilter::GaussianAnalogFilter(unsigned int kernel_length, double std_dev) : AnalogFilter(kernel_length) {
  m_std_dev = std_dev;
  if (std_dev < 0)
    m_std_dev = 0.0;

  // Register standard-deviation as a valid parameter
  params["std_dev"] = &m_std_dev;
}


/** Calculates a new gaussian filter kernel for the given number of data points. n must not be longer than the kernel length.
    \param n Number of data points to filter. */
void GaussianAnalogFilter::createKernel(unsigned int n) {
  if (n <= 0) {
    TheCsimError.add("GaussianAnalogFilter::createKernel: Invalid number of sample points %i\n", n);
    return;
  }

  // Do not allow longer kernels
  if (n > nKernelLength)
    n = nKernelLength;

  double tmp_EPSILON = 10e-20;
  if (m_std_dev < tmp_EPSILON) {
    // Take only the current data value, do not filter
    for (int i=0; i<(n-1); i++) {
      filter_kernel[i] = 0;
    }
    filter_kernel[n-1] = 1.0;
  }

  else {
    // Calculate weights of gaussian kernel
    double gauss_sum = 0.0;
    double variation = m_std_dev * m_std_dev;
    double sqr_dist = 0;
	int i;
    for (i=0; i<n; i++) {
      sqr_dist = (n-i-1) * (n-i-1);
      filter_kernel[i] = exp(-sqr_dist / (2*variation));
      gauss_sum += filter_kernel[i];
    }

    // normalize kernel weights
    for (i=0; i<n; i++) {
      filter_kernel[i] /= gauss_sum;
    }
  }

}

