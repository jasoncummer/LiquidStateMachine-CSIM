/** \file useranalogfilter.cpp
**  \brief Analog low-pass filter with a user defined kernel.
*/

#include "useranalogfilter.h"


/** Constructs a new user-defined analog filter.
    \param kernel_length The length of the filter kernel.
    \param weights An array of filter-weights. These weights are not normalized,
    so make sure that they sum up to 1, if you want to create a low-pass filter. */
UserAnalogFilter::UserAnalogFilter(unsigned int kernel_length, double *weights) : AnalogFilter(kernel_length) {

  // Do not register the weights, they cannot be changed online

  m_weights = new double[kernel_length];

  if (weights == 0) {
    // No weights specified, make it a one-point filter
    for (int i=0; i<kernel_length; i++)
      filter_kernel[i] = m_weights[i] = 0;

    filter_kernel[kernel_length-1] = m_weights[kernel_length-1] = 1.0;
    m_sum_kernel = 1;
  }
  else {
    for (int i=0; i<kernel_length; i++) {
      filter_kernel[i] = m_weights[i] = weights[i];
      m_sum_kernel += weights[i];
    }
  }
}

/** Free the memory. */
UserAnalogFilter::~UserAnalogFilter() {
  if (m_weights)
    delete[] m_weights;
  m_weights = 0;
}


/** Calculates a new kernel for the given number of data points. If n is not equal to the kernel-length,
    then the last n weights of the user-defined kernel are used. Their sum is normalized to the sum of
    the whole kernel. n is set to the kernel length if n > kernel-length.
    \param n Number of data points to filter. */
void UserAnalogFilter::createKernel(unsigned int n) {

  int i;

  if (n <= 0) {
    TheCsimError.add("UserAnalogFilter::createKernel: Invalid number of sample points %i\n", n);
    return;
  }

  // Longer n than kernel-length does not make sense
  if (n > nKernelLength)
    n = nKernelLength;

  double f_sum = 0;
  int pos = 0;
  // Take the last n weights of the kernel
  for ( i=0; i<nKernelLength; i++) {
    pos = nKernelLength-n+i;
    if ((pos < 0) || (pos >= nKernelLength))
      // Fill up with zeros
      filter_kernel[i] = 0;
    else
      filter_kernel[i] = m_weights[nKernelLength-n+i];
    f_sum += filter_kernel[i];
  }

  const double TMP_EPSILON = 10e-20;
  if ((fabs(f_sum) > TMP_EPSILON) && (fabs(m_sum_kernel) > TMP_EPSILON)) {
    // normalize the sum of weights if it is not zero
    for (i=0; i<n; i++) {
      filter_kernel[i] *= (m_sum_kernel / f_sum);
    }
  }

  double sum = 0;
  for (i=0; i<n; i++) {
    sum += filter_kernel[i];
  }


}


/** Updates the internal state of the filter. */
int UserAnalogFilter::updateInternal() {
  // Do not allow changing the kernel-length

  /*  int updatedKernelLength = (int) dKernelLength;

  if (oldKernelLength != nKernelLength) {
    TheCsimError.add("UserAnalogFilter::updateInternal: Changing the size of the kernel is not allowed for user-defined filters! Create a new filter instead!");
    dKernelLength = (double) oldKernelLength;
    return -1;
    } */

  //  oldKernelLength = nKernelLength;

  m_sum_kernel = 0;
  for (int i=0; i<nKernelLength; i++) {
    filter_kernel[i] = m_weights[i];
    m_sum_kernel += filter_kernel[i];
  }

  return 0;
}

