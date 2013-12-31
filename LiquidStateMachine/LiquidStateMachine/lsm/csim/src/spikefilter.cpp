/** \file spikefilter.cpp
**  \brief Base class for all spike-filtering functions.
*/

#include "spikefilter.h"


/** Constructs a new spike filter. 
    \param values_per_channel Number of values to store per channel (e.g. 1 for exp-filter, 2 for alpha-filter). */
SpikeFilter::SpikeFilter(unsigned int values_per_channel) {
  m_dt = 1e-4;
  nChannels = 0;
  lChannels = 0;

  nValuesPerChannel = values_per_channel;
  lastValues = 0;
  firstSpike = 0;
  p_spikeSource = 0;
  
  m_simtime = 0;

  // Register parameters in map
  params["dt"] = &m_dt;
}


/** Frees the memory. */
SpikeFilter::~SpikeFilter() {
  if (lastValues) {
    for (int i=0; i<nChannels; i++) {
      delete[] lastValues[i];
      lastValues[i] = 0;
    }
    free(lastValues);
    lastValues = 0;
  }
  if (firstSpike)
    free(firstSpike);
  firstSpike = 0;
  if (p_spikeSource)
    free(p_spikeSource);
  p_spikeSource = 0;

}


/** Resets the information stored within the filter. */
void SpikeFilter::reset() {
  int i, j;

  // Reset filter values
  if (lastValues) {
    for (i=0; i<nChannels; i++) {
      for (j=0; j<nValuesPerChannel; j++)
	lastValues[i][j] = 0.0;
    }
  }
  
  // Reset first spike
  if (firstSpike) {
    for (i=0; i<nChannels; i++)
      firstSpike[i] = 0;
  }

  m_simtime = 0;
}



/** Adds one input channel.
    \param o Pointer to spiking neuron. The "spikes" channel of this neuron is inserted. */
void SpikeFilter::addChannel(csimClass *o) {
  int i;
  SpikingNeuron *n = dynamic_cast<SpikingNeuron *>(o);

  // Allocate memory
  if (n) {
    if ( ++nChannels > lChannels ) {
      lChannels += 10;
      lastValues = (double **) realloc(lastValues, lChannels*sizeof(double *));
      for (i=0; i<10; i++)
	lastValues[lChannels-10+i] = 0;
      firstSpike = (int *) realloc(firstSpike, lChannels*sizeof(int));
      p_spikeSource = (SpikingNeuron **) realloc(p_spikeSource, lChannels*sizeof(SpikingNeuron *));
    }
    
    
    // Set initial values
    lastValues[nChannels-1] = new double[nValuesPerChannel];
    for (i=0; i<nValuesPerChannel; i++)
      lastValues[nChannels-1][i] = 0;
    firstSpike[nChannels-1] = 0;
    p_spikeSource[nChannels-1] = n;

  }
  else {
    TheCsimError.add("SpikeFilter::add_channel: Can only view channels from SpikingNeurons!\n");
  }
}


 
/** Filter a response signal of the neural microcircuit.
    \param R Response of the neural microcircuit. (not needed for spikes)
    \param X Target vector where to save the results. 
    \param indices Indices where to store the results in X.
    \return -1 if an error occured, 1 for success. */
int SpikeFilter::filter(const double* R, double* X, int* indices = 0) {

  if (X == 0) {
    TheCsimError.add("SpikeFilter::filter: Target vector is a NULL pointer!\n");
    return -1;
  }

  // Set pointer to begin of storage memory
  double *x;

  if (indices) {
    // Indirect addressation
    x = X + indices[0];
    for (int i=0; i<(nChannels-1); i++) {
      
      processSpike(i, x);
      x += (indices[i+1] - indices[i]);
    }
    processSpike(nChannels-1, x);
  }
  else {
    x = X;
    for (int i=0; i<nChannels; i++) {
      processSpike(i, x);
      x++;
    }
  }


  // Advance simulation time
  m_simtime += m_dt;

  return 1;
}

