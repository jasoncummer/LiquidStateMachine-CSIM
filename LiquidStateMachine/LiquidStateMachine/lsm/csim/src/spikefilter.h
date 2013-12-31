/** \file spikefilter.h
**  \brief Base class for all spike-filtering functions.
*/

#ifndef _SPIKEFILTER_H_
#define _SPIKEFILTER_H_

#include "filterfunction.h"
#include "spikingneuron.h"
#include "csimclass.h"

using namespace std;

//! Base class of all spike filters
class SpikeFilter : public FilterFunction {

 public:
  /** Constructs a new spike filter. 
      \param values_per_channel Number of values to store per channel (e.g. 1 for exp-filter, 2 for alpha-filter). */
  SpikeFilter(unsigned int values_per_channel=1);

  virtual ~SpikeFilter(void);

  /** Filter a spike signal of the neural microcircuit.
    \param r Response of the neural microcircuit. (not needed for spikes)
    \param x Target vector where to save the results. 
    \param indices Indices where to store the results in X.
    \return -1 if an error occured, 1 for success. */
  virtual int filter(const double* r, double* x, int* indices);

  /** Resets the information stored within the filter. */
  virtual void reset();

  /** Adds one input channel. 
      \param o Pointer to spiking neuron. The "spikes" channel of this neuron is inserted. */
  virtual void addChannel(csimClass *o);

 protected:
  /** Process the occurence of a spike. 
      \param channel Index of the channel to filter.
      \param target Target value where to store the new filtered value. */
  virtual double processSpike(int channel, double *target) = 0;



  //! Number of Input channels to filter.
  int nChannels;
  //! Number of allocated input channels. \internal [hidden;]
  int lChannels;

  //! Time step length.
  double m_dt;

  //! Current time. \internal [hidden;]
  double m_simtime;

  //! Last filtered values of all channels. \internal [hidden;]
  double **lastValues;
  //! Index of the first spike in the time window. \internal [hidden;]
  int *firstSpike;
  //! Pointers to the spiking neurons.
  SpikingNeuron **p_spikeSource;

  //! Number of values stored for each channel in lastValues. [range=(1,2);]
  int nValuesPerChannel;

};

#endif
