/** \file analogfilter.h
**  \brief Abstract base class of all analog low-pass filters.
*/

#ifndef _ANALOGFILTER_H_
#define _ANALOGFILTER_H_

#include <deque>
#include "filterfunction.h"

using namespace std;

//! Base class of all analog filters
class AnalogFilter : public FilterFunction {

 public:
  /** Constructs a new analog filter.
      \param kernel_length The length of the filter kernel. */
  AnalogFilter(unsigned int kernel_length);

  virtual ~AnalogFilter(void);

  /** Filter a response signal of the neural microcircuit.
      \param r Response of the neural microcircuit.
      \param x Target vector where to save the results. 
      \param indices Indices where to store the results in X.
      \return -1 if an error occured, 1 for success. */
  virtual int filter(const double* r, double* x, int* indices = 0);

  /** Calculates a new filter kernel for the given number of data points.
      \param n Number of data points to filter. */
  virtual void createKernel(unsigned int n) = 0;

  /** Resets the information stored within the filter. */
  virtual void reset();

  /** Sets the number of input channels.
      \param nInputs The number of inputs. 
      \return -1 if an error occured, 1 for success. */
  virtual int setNumInputs(unsigned int nInputs);

  /** Adds one input channel. */
  virtual void addChannel();

  /** This function is called after parameters are updated. */
  virtual int updateInternal();

  //! The length of the kernel.
  int nKernelLength;

 protected:

  //! The filter kernel. \internal [hidden;]
  double* filter_kernel;

  //! The length of the old kernel. \internal [hidden;]
  int oldKernelLength;

  //! Number of Input channels to filter.
  int nChannels;
  //! Number of allocated input channels. \internal [hidden;]
  int lChannels;

  //! Storage of the last datapoints for low-pass filtering. \internal [hidden;]
  deque<double> **dataQueues;

  //! Number of data-points received. \internal [hidden;]
  int nInputAvailable;

  //! Dummy variable for parameter setting and getting. \internal [hidden;]
  double dKernelLength;
};

#endif

