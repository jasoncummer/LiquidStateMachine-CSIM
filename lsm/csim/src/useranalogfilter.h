/** \file useranalogfilter.h
**  \brief Analog low-pass filter with a user defined kernel.
*/

#ifndef _USERANALOGFILTER_H_
#define _USERANALOGFILTER_H_

#include "analogfilter.h"
#include "csimclass.h"
#include <math.h>

using namespace std;

//! Implementation of an analog low-pass filter with a user-defined kernel (maximum length = 10).
class UserAnalogFilter : public AnalogFilter {

  DO_REGISTERING

 public:
  /** Constructs a new user-defined analog filter.
      \param kernel_length The length of the filter kernel. 
      \param weights An array of filter-weights. These weights are not normalized, 
             so make sure that they sum up to 1, if you want to create a low-pass filter. */
  UserAnalogFilter(unsigned int kernel_length=10, double *weights=0);

  ~UserAnalogFilter(void);

  /** Calculates a new kernel for the given number of data points. If n is not equal to the kernel-length,
      then the last n weights of the user-defined kernel are used. Their sum is normalized to the sum of 
      the whole kernel.
      \param n Number of data points to filter. */
  void createKernel(unsigned int n);

  /** Updates the internal state of the filter. */
  int updateInternal();

  //! User-defined weights. [size=10;]
  double *m_weights;

 private:
  /** Sum of all kernel weights. */
  double m_sum_kernel;
};

#endif
