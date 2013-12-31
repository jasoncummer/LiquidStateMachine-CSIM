/** \file triangularanalogfilter.cpp
**  \brief Analog triangular low-pass filter.
*/

#include "triangularanalogfilter.h"


/** Constructs a new triangular analog filter.
    \param kernel_length The length of the filter kernel. */
TriangularAnalogFilter::TriangularAnalogFilter(unsigned int kernel_length) : AnalogFilter(kernel_length) {
}


/** Calculates a new triangular filter kernel for the given number of data points. n must not be longer than the kernel length.
    \param n Number of data points to filter. */
void TriangularAnalogFilter::createKernel(unsigned int n) {
  if (n <= 0) {
    TheCsimError.add("TriangularAnalogFilter:createKernel: Invalid number of sample points %i\n", n); 
    return;
  }

  double kernel_step = 2.0 / ((double) (n+1) * (double) n);

  // Do not allow longer kernels
  if (n > nKernelLength)
    n = nKernelLength;

  for (int i=0; i<n; i++) {
    filter_kernel[i] = (double) (i+1) * kernel_step;
  }
}
