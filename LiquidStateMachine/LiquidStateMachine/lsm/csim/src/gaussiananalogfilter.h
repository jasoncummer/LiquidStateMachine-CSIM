/** \file gaussiananalogfilter.h
**  \brief Analog gaussian low-pass filter.
*/

#ifndef _GAUSSIANANALOGFILTER_H_
#define _GAUSSIANANALOGFILTER_H_

#include "analogfilter.h"
#include "csimclass.h"

using namespace std;

//! Implementation of an analog low-pass filter with a gaussian kernel.
class GaussianAnalogFilter : public AnalogFilter {

  DO_REGISTERING

 public:
  /** Constructs a new gaussian analog filter.
      \param kernel_length The length of the filter kernel. 
      \param std_dev Standard deviation of the filter mask. */
  GaussianAnalogFilter(unsigned int kernel_length=1, double std_dev=1.0);

  ~GaussianAnalogFilter(void) {};

  /** Calculates a new gaussian kernel for the given number of data points.
      \param n Number of data points to filter. */
  void createKernel(unsigned int n);

  //! Standard deviation of the filter mask.
  double m_std_dev;

};

#endif

