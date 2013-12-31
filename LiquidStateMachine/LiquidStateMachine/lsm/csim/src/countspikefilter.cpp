/** \file countspikefilter.cpp
**  \brief Filter which counts the spikes in a given time window.
*/

#include "countspikefilter.h"


/** Constructs a new spike counter.
    \param time_window Length of time window in ms (default 1 sec). */
CountSpikeFilter::CountSpikeFilter(double p_time_window) : SpikeFilter(1) {

  // Store and register the time window
  time_window = p_time_window;

  params["time_window"] = &time_window;
}

/** Updates the internal state of the filter. */
int CountSpikeFilter::updateInternal() {
  m_dt = DT;
  return 0;
}

/** Process the occurence of a spike. 
    \param channel Index of the channel to filter.
    \param target Target value where to store the new filtered value. */
double CountSpikeFilter::processSpike(int channel, double *target) {

  if ((channel < 0) || (channel >= nChannels)) {
    TheCsimError.add("CountSpikeFilter::processSpike: Invalid channel index %i!\n", channel); 
    return -1;
  }

  // Filtered value of this field in the last time step
  double old_value = lastValues[channel][0];
  double filtered_value = old_value;
  double cur_time = SimulationTime;

  // Begin of time window
  double begin_window_time = cur_time - time_window;
  if (begin_window_time < 0)
    begin_window_time = 0;

  // End of time window
  double end_window_time = cur_time - m_dt;

  // Take the values directly from the spiking neuron, where they are stored
  // (dirty, but memory efficient)

  SpikingNeuron *n = p_spikeSource[channel];

  // Number of spikes in this channel
  int nSpikes = n->nSpikes();


  // Get index of first spike in last time window
  int spike_ind = firstSpike[channel];

  if ((spike_ind >= nSpikes) || (nSpikes <= 0)) {
    // No spikes in time window
    lastValues[channel][0] = 0;
    *target = 0;
    return 0.0;
  }
    
  double spikeTime = n->spikeTime(spike_ind);

  // Delete all spikes that fall out of the time window
  while ((spikeTime < begin_window_time) && (spike_ind < nSpikes)) {
    filtered_value -= 1.0;
    spike_ind = ++(firstSpike[channel]);
    if (spike_ind < nSpikes)
      spikeTime = n->spikeTime(spike_ind);
  }

  // Find time of most recent spike
  spike_ind = n->nSpikes()-1;
  if (spike_ind >= 0) {
    spikeTime = n->spikeTime(spike_ind);
      
    // Add all spikes that occured in the last time step (should be max. 1)
    while ((spikeTime > end_window_time) && (spike_ind >= 0)){
      //      csimPrintf("Found spike %d at %g (@ %g < %g)\n", spike_ind, spikeTime, end_window_time, cur_time);
      filtered_value += 1.0;
      spike_ind--;
      if (spike_ind >= 0)
	spikeTime = n->spikeTime(spike_ind);
    }
  }

  // Store the new filtered value
  lastValues[channel][0] = filtered_value;
  *target = filtered_value;

  return filtered_value;
}





/*
int main() {
  CountSpikeFilter *csf = new CountSpikeFilter(0.5, 30);

  cout << "Vor getparameter\n";

  list<string> par_names = csf->validParameters();
  list<string>::iterator p = par_names.begin();

  while (p!=par_names.end()) {
    cout << *p << ": " << csf->getParameter(*p) << "\n";
    p++;
  }

  csf->setParameter("jupidu", 0.32);

  csf->setParameter("kernelsize", 13.0);

  csf->setParameter("time_window", 20);

  par_names = csf->validParameters();
  p = par_names.begin();

  while (p!=par_names.end()) {
    cout << *p << ": " << csf->getParameter(*p) << "\n";
    p++;
  }


  delete csf;
}

*/
