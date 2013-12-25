#ifndef _NEEDSPOSTSPIKESYNAPSE_H_
#define _NEEDSPOSTSPIKESYNAPSE_H_

//! Interface for synapses which will receive a postsynaptic spike
class NeedsPostSpikeSynapse {

public:
  //! Called if the postsynaptic spikes hits (arrives at) the synapse
  virtual void postSpikeHit(void)=0;
  
};

#endif
