/** \file analogfilter.cpp
**  \brief Analog low-pass filter.
*/

#include "analogfilter.h"


/** Constructs a new analog filter.
    \param kernel_length The length of the filter kernel. */
AnalogFilter::AnalogFilter(unsigned int kernel_length) {
  if (kernel_length > 0) {
    nKernelLength = oldKernelLength = kernel_length;
    filter_kernel = new double[nKernelLength];
  }
  else {
    // One-point kernel
    nKernelLength = oldKernelLength = 1;
    filter_kernel = new double[1];
    filter_kernel[0] = 1.0;
  }

  nChannels = 0;
  lChannels = 0;
  dataQueues = 0;

  nInputAvailable = 0;

  // Register parameters in map
  dKernelLength = (double) nKernelLength;
  params["kernelsize"] = &dKernelLength;
}

/** Frees the memory. */
AnalogFilter::~AnalogFilter() {
  if (filter_kernel)
    delete[] filter_kernel;
  filter_kernel = 0;


  if (dataQueues) {
    for (int i=0; i<nChannels; i++) {
      dataQueues[i]->clear();
      delete dataQueues[i];
      dataQueues[i] = 0;
    }
    free(dataQueues);
  }

  dataQueues = 0;
}


/** Updates the internal state of the filter. */
int AnalogFilter::updateInternal() {
  /*
  int oldKernelLength = nKernelLength;
  nKernelLength = (int) dKernelLength;
  if (nKernelLength < 1) {
    nKernelLength = 1;
    dKernelLength = 1;
  }
  */

  // Reallocate memory for kernel
  if (oldKernelLength != nKernelLength) {
    delete[] filter_kernel;
    filter_kernel = new double[nKernelLength];
    createKernel(nKernelLength);
    oldKernelLength = nKernelLength;
  }

  return 0;
}


/** Resets the information stored within the filter. */
void AnalogFilter::reset() {

  // Clear all stored data points
  if (dataQueues) {
    for (int i=0; i<nChannels; i++) {
      dataQueues[i]->clear();
    }
  }

  nInputAvailable = 0;
}

/** Sets the number of input channels.
    \param nInputs The number of inputs.
    \return -1 if an error occured, 1 for success. */
int AnalogFilter::setNumInputs(unsigned int nInputs) {
  if (nInputs > 0) {
    nChannels = nInputs;
    lChannels = nInputs;
    dataQueues = (deque<double> **)realloc(dataQueues,nInputs*sizeof(deque<double>  *));

    for (int i=0; i<nChannels; i++) {
      dataQueues[i] = new deque<double>();
    }
  }
  else {
    TheCsimError.add("AnalogFilter::setNumInputs: Number of channels must be > 0!\n");
    return -1;
  }
}

/** Adds one input channel. */
void AnalogFilter::addChannel() {
  if ( ++nChannels > lChannels ) {
    lChannels += 10;
    dataQueues = (deque<double> **)realloc(dataQueues,lChannels*sizeof(deque<double>  *));
  }
  dataQueues[nChannels-1] = new deque<double>();
}


#ifdef _WIN32
#define min(a,b) ( (a) < (b) ? (a) : (b) )
#endif

/** Filter a response signal of the neural microcircuit.
    \param R Response of the neural microcircuit.
    \param X Target vector where to save the results.
    \param indices Indices where to store the results in X.
    \return -1 if an error occured, 1 for success. */
int AnalogFilter::filter(const double* R, double* X, int* indices) {

  if (R == 0) {
    TheCsimError.add("AnalogFilter::filter: Input is a NULL pointer!\n");
    return -1;
  }
  if (X == 0) {
    TheCsimError.add("AnalogFilter::filter: Target vector is a NULL pointer!\n");
    return -1;
  }

  deque<double>::iterator p;
  double f_value;
  int i, j;

  nInputAvailable++;

  if ((nInputAvailable) <= nKernelLength) {
    // Length of collected input data is shorter than
    // desired size of filter kernel
    // Calculate a new shorter kernel

    createKernel(nInputAvailable);
  }

  // Put the new data into the queues
  for (i=0; i<nChannels; i++) {
    // Delete the oldest element in the queue
    if (nInputAvailable > nKernelLength)
      dataQueues[i]->pop_front();
    // Add the new value at the end of the queue
    dataQueues[i]->push_back(R[i]);
  }

  int nToFilter = min(nInputAvailable, nKernelLength);


  // Filter all analog channels
  for (i=0; i<nChannels; i++) {
    p = dataQueues[i]->begin();
    f_value = 0.0;

    for (j=0; j<nToFilter; j++) {
      // Calculate filtered value
      f_value += filter_kernel[j] * *p;

      if (p != dataQueues[i]->end()) {
	if (j < (nToFilter - 1))
	  // Do not advance the iterator for the last object, since we want
	  // to change its content
	  p++;
      }
      else {
	TheCsimError.add("AnalogFilter::filter: Data was lost before filtering!\n");
	return -1;
      }
    }

    // Store the filtered value: Replace the value of the last input
    *p = f_value;
    if (indices)
      X[indices[i]] = f_value;
    else
      X[i] = f_value;
  }

  return 1;
}
