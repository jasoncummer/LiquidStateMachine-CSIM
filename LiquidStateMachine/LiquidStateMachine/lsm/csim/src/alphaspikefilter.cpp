/** \file alphaspikefilter.cpp
**  \brief Filter which simulates spikes with alpha functions.
*/

#include "alphaspikefilter.h"
#include <math.h>


/** Constructs a new alpha-function spike filter.
    \param tau1 Time constant (default \f$tau_1 = 0.03\f$). Decay by \f$alpha(-DT/\tau_1)\f$.
    \param tau2 Time constant (default \f$tau_1 = 0.003\f$). Decay by \f$alpha(-DT/\tau_2)\f$. */
AlphaSpikeFilter::AlphaSpikeFilter(double tau1, double tau2) : SpikeFilter(2) {

  // Store and register the decay constants
  m_tau1 = tau1;
  m_tau2 = tau2;

  params["tau1"] = &m_tau1;
  params["tau2"] = &m_tau2;
  m_decay1 = exp(-m_dt/m_tau1);
  m_decay2 = exp(-m_dt/m_tau2);
}

/** Updates the internal state of the filter. */
int AlphaSpikeFilter::updateInternal() {
  m_dt = DT;
  m_decay1 = exp(-m_dt/m_tau1);
  m_decay2 = exp(-m_dt/m_tau2);
  return 0;
}

/** Process the occurence of a spike. 
    \param channel Index of the channel to filter.
    \param target Target value where to store the new filtered value. */
double AlphaSpikeFilter::processSpike(int channel, double *target) {

  if ((channel < 0) || (channel >= nChannels)) {
    TheCsimError.add("AlphaSpikeFilter::processSpike: Invalid channel index %i!\n", channel); 
    return -1;
  }

  // Filtered values of this field in the last time step
  double old_value_1 = lastValues[channel][0];
  double old_value_2 = lastValues[channel][1];
  double filtered_value_1 = old_value_1;
  double filtered_value_2 = old_value_2;
  double filtered_value = filtered_value_1 - filtered_value_2;
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
    lastValues[channel][0] = old_value_1 * m_decay1;
    lastValues[channel][1] = old_value_2 * m_decay2;
    *target = lastValues[channel][0] - lastValues[channel][1];
    return lastValues[channel][0] - lastValues[channel][1];
  }
    
  double spikeTime = n->spikeTime(spike_ind);
  double delta;

  const double TMP_EPSILON = 10e-20;
  
  // Add all spikes that occured in the last time step (should be max. 1)
  while ((spikeTime <= cur_time) && (spike_ind < nSpikes)){
    delta = spikeTime - cur_time + m_dt;

    // Optimization: Dont calculate the decay again if delta==DT
    if ((delta < (m_dt + TMP_EPSILON)) && (delta > (m_dt - TMP_EPSILON))) {
      filtered_value_1 = filtered_value_1 * m_decay1 + 1.0;
      filtered_value_2 = filtered_value_2 * m_decay2 + 1.0;
    }
    else {
      filtered_value_1 = filtered_value_1 * exp(-delta/m_tau1) + 1.0;
      filtered_value_2 = filtered_value_2 * exp(-delta/m_tau2) + 1.0;
    }

    spike_ind = ++(firstSpike[channel]);
    if (spike_ind < nSpikes)
      spikeTime = n->spikeTime(spike_ind);
  }
  
  // Store the new filtered value
  lastValues[channel][0] = filtered_value_1;
  lastValues[channel][1] = filtered_value_2;
  filtered_value = filtered_value_1 - filtered_value_2;
  *target = filtered_value;
  return filtered_value;
}
