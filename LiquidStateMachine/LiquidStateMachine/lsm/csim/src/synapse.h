/*! \file synapse.h
**  \brief Class definition of Synapse
*/

#ifndef _SYNAPSE_H_
#define _SYNAPSE_H_

#include "advanceable.h"
#include "neuron.h"

class SynapseTarget;
class Neuron;

//! Base class of all synapses
/** This class takes care about the connectivity: it maintains a
 ** pointer to the presynaptic and the postsynaptic object as well to the summation point.
  */
class Synapse : public Advancable {
 public:
 
  Synapse(void);

  //! Connects the synapse to the postsynaptic object which must be a synaptic target
  virtual int addOutgoing(Advancable *a);

  //! Connects the synapse to the presynaptic object.
  virtual int addIncoming(Advancable *a);

  //! Get the Id of the presynaptic object
  inline uint32 getPre(void)  { return pre->getId();  };

  //! Get the Id of the postsynaptic object
  inline uint32 getPost(void) { return post->getId(); };

  //! The weight (scaling factor, strenght, maximal amplitude) of the synapse \internal [readwrite; units=;]
  float W;

  //! The synaptic transmission delay \internal [readwrite; units=sec; range=(0,1);]
  /** As the synaptic transmission delay we understand the time
   ** between a signal (spike or analog signal) is generated in the
   ** presynaptic object and the time of the onset of the postsynaptic
   ** response. */
  float delay;

  //! The psr (postsynaptic response) is the result of whatever computation is going on in a synapse. \internal [units=; readonly;]
  double psr;

 protected:
  
  //! Pointer to postsynaptic summation point within the \link SynapseTarget synaptic target \endlink ( Neuron, SynapticChannel, Compartment...) \internal [hidden;]
  double *summationPoint;

  //! Pointer to presynaptic object
  Advancable *pre;

  //! Pointer to postsynaptic object
  Advancable *post;
};

#endif
