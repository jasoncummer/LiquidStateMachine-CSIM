/** \file triangularanalogfilter.h
**  \brief Analog triangular low-pass filter.
*/

#ifndef _TRIANGULARANALOGFILTER_H_
#define _TRIANGULARANALOGFILTER_H_

#include "analogfilter.h"
#include "csimclass.h"

using namespace std;

//! Implementation of an analog low-pass filter with a triangular kernel.
class TriangularAnalogFilter : public AnalogFilter {

  DO_REGISTERING

 public:
  /** Constructs a new triangular analog filter.
      \param kernel_length The length of the filter kernel. */
  TriangularAnalogFilter(unsigned int kernel_length = 1);

  ~TriangularAnalogFilter(void) {};

  /** Calculates a new triangular kernel for the given number of data points.
      \param n Number of data points to filter. */
  void createKernel(unsigned int n);

};

#endif
