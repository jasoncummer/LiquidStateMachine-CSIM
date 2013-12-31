/*! \file spikingneuron.h
**  \brief Class definition of SpikingNeuron
*/

#ifndef _SPIKINGNEURON_H_
#define _SPIKINGNEURON_H_

#include "neuron.h"
#include "csimlist.h"
#include "spikingsynapse.h"


//! Base class for all spiking neurons, i.e. spike emitting objects
/**
 ** It maintains a list of emitted spikes. Outgoing synapses are
 ** granted to have access to this list to implement arbitrary long
 ** transmission delays. This is a memory efficient implementation
 ** since it would need more memory (for long delays)to store the
 ** spikes at each individual synapse.
 **
*/
class SpikingNeuron : public Neuron { 

 public:
  
  SpikingNeuron(void);

  //! The destructor  clears the list of spikes
  virtual ~SpikingNeuron();

  //! Reset the SpikingNeuron
  virtual void reset(void);

  virtual double nextstate(void) { 
    outSpike = hasFired; 
    if ( hasFired ) saveSpike();
    return hasFired;
  };

  virtual void force(double s) {
    outSpike = s;
  };

  virtual void output(void) { 
    if ( outSpike ) propagateSpike(); 
  };

  //! This method should return 1 (0) if the spiking neuron fired a spike during the current time step.
  virtual int fired(void) { return hasFired; };

  //! This method should return 1 (0) if the spiking neuron is in its absolute refractory period.
  virtual int isRefractory(void) { return 0; };

  //! Return the number of spikes the object has emitted since t=0
  inline int nSpikes(void) { return nEmittedSpikes; };

  //! Return the time of the i-th spike
  inline double spikeTime(int i) { return Spikes[i]; };

  //! Copy the array of spike times to the given buffer
  void copySpikes(double *buffer) {
    memcpy(buffer,Spikes,sizeof(double)*nEmittedSpikes);
  }

  //! Add an incoming synapse S
  virtual int addIncoming(Advancable *S);

  //! Add an outgoing synapse S and check whether S is derived from SpikingSynapse
  virtual int addOutgoing(Advancable *S);

 protected:
  //! Should be set to true (false) during an implementation of nextstate() if the neuron fires (does not fire)
  bool hasFired;


 private:
  // SpikingInputNeuron will be a friend bcs. it will call
  // propagateSpike ans saveSpike directly such that it can not be
  // forced.
  friend class SpikingInputNeuron;

  //! If true (false) all spike detinations will be informed about a spike at SimulationTime
  bool outSpike;

  //! Notifies all spike destination about the spike at SimulationTime
  void propagateSpike(void);

  //! Add a spike at SimulationTime to the internal spike time list
  void saveSpike(void);

  //! An array of all spikes emitted \internal [hidden]
  double *Spikes;

  //! Number of spikes emited \internal [readonly; units=;]
  int nEmittedSpikes;

  //! Number of spikes which can currently be hold by the array. \internal [hidden]
  int nSpikesAlloc;

  //! Structure holding all information necessay to rout a spike to its target
  struct SpikeDest {
    SpikeDest(SpikingSynapse *s, pSpikeHandler h, float *d) {synapse=s; spikeHandler=h; delay=d;}
    SpikingSynapse *synapse;
    pSpikeHandler  spikeHandler;   // pointer at member function which handles the spike
    float *delay;
  };

  csimList<SpikeDest,100> SpikeReceiver;
};

#endif
