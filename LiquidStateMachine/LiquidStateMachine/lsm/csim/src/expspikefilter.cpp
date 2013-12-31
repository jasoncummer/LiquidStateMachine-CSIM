/** \file expspikefilter.cpp
**  \brief Filter which simulates exponential decay of spikes.
*/

#include "expspikefilter.h"
#include <math.h>


/** Constructs a new exponential-decay filter.
    \param tau1 Time constant (default \f$tau_1 = 0.03\f$). Decay by \f$exp(-DT/\tau_1)\f$. */
ExpSpikeFilter::ExpSpikeFilter(double tau1) : SpikeFilter(1) {

  // Store and register the decay constant
  m_tau1 = tau1;

  params["tau1"] = &m_tau1;
  m_decay = exp(-m_dt/m_tau1);
}

/** Updates the internal state of the filter. */
int ExpSpikeFilter::updateInternal() {
  m_dt = DT;
  m_decay = exp(-m_dt/m_tau1);
  return 0;
}

/** Process the occurence of a spike. 
    \param channel Index of the channel to filter.
    \param target Target value where to store the new filtered value. */
double ExpSpikeFilter::processSpike(int channel, double *target) {

  if ((channel < 0) || (channel >= nChannels)) {
    TheCsimError.add("ExpSpikeFilter::processSpike: Invalid channel index %i!\n", channel); 
    return -1;
  }

  // Filtered value of this field in the last time step
  double old_value = lastValues[channel][0];
  double filtered_value = old_value;
  double cur_time = m_simtime;

  // Take the values directly from the spiking neuron, where they are stored
  // (dirty, but memory efficient)

  SpikingNeuron *n = p_spikeSource[channel];

  // Number of spikes in this channel
  int nSpikes = n->nSpikes();

  // Get index of next spike to process
  int spike_ind = firstSpike[channel];

  if ((spike_ind >= nSpikes) || (nSpikes <= 0)) {
    // No new spike, only decay
    lastValues[channel][0] = old_value * m_decay;
    *target = lastValues[channel][0];
    return lastValues[channel][0];
  }
    
  double spikeTime = n->spikeTime(spike_ind);
  double delta;

  const double TMP_EPSILON = 10e-20;
  
  // Add all spikes that occured in the last time step (should be max. 1)
  while ((spikeTime <= cur_time) && (spike_ind < nSpikes)){
    delta = spikeTime - cur_time + DT;

    // Optimization: Dont calculate the decay again if delta==DT
    if ((delta < (m_dt + TMP_EPSILON)) && (delta > (m_dt - TMP_EPSILON)))
      filtered_value = filtered_value * m_decay + 1.0;
    else
      filtered_value = filtered_value * exp(-delta/m_tau1) + 1.0;

    spike_ind = ++(firstSpike[channel]);
    if (spike_ind < nSpikes)
      spikeTime = n->spikeTime(spike_ind);
  }
  
  // Store the new filtered value
  lastValues[channel][0] = filtered_value;
  *target = filtered_value;

  return filtered_value;
}
