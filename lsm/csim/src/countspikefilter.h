/** \file countspikefilter.h
**  \brief Filter which counts the spikes in a given time window.
*/

#ifndef _COUNTSPIKEFILTER_H_
#define _COUNTSPIKEFILTER_H_

#include "spikefilter.h"

using namespace std;

//! Filter which counts the spikes in a given time window.
class CountSpikeFilter : public SpikeFilter {

  DO_REGISTERING

 public:
  /** Constructs a new spike counter.
      \param time_window Length of time window in ms (default 1 sec). */
  CountSpikeFilter(double time_window = 1000.0);

  ~CountSpikeFilter(void) {};

  /** Updates the internal state of the filter. */
  int updateInternal();

  //! Length of time window.
  double time_window;

 protected:
  /** Process the occurence of a spike. 
      \param channel Index of the channel to filter.
      \param target Target value where to store the new filtered value. */
  double processSpike(int channel, double *target);


};

#endif
