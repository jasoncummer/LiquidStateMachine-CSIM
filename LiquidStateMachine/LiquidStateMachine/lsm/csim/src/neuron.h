/*! \file neuron.h
**  \brief Class definition of Neuron
*/

#ifndef _NEURON_H_
#define _NEURON_H_

#include "forceable.h"
#include "csimlist.h"
#include "synapsetarget.h"

class Synapse;

//! Base class of all neurons
/** It maintains arrays for incoming and outgoing synapses which can
 ** be added by addIncoming() and addOutgoing(). Other neuron types
 ** derived from this class should reimplement addIncoming() and
 ** addOutgoing() to check wheter we are connecting allowable synapse
 ** types.  */
class Neuron : public Forceable, public SynapseTarget { 

 public:

  // The constructor.
  Neuron(void);

  // The destructor.
  virtual ~Neuron();

  //! Reset the  neuron.
  virtual void reset(void) { summationPoint = 0.0; };

  //! Add an incoming synapse.
  virtual int addIncoming(Advancable *s);

  //! Add an outgoing synapse.
  virtual int addOutgoing(Advancable *s);

  //! Type (e.g. inhibitory or excitatory) of the neuron \internal [units=;]
  int type;

  inline uint32 nPre(void)  { return nIncoming; }
  inline uint32 nPost(void) { return nOutgoing; }
  void getPre(uint32 *idx);
  void getPost(uint32 *idx);

 protected:

  //! A list of incoming synapses
  Synapse **incoming;

  //! Number of incoming synapses \internal [readonly; units=;]
  int nIncoming;

  //! Current size  of array (in number of synapses) allocated for incoming synapses \internal [hidden]
  int nIncomingAlloc;

  //! An array of outgoing synapses
  Synapse **outgoing;

  //!  Number of outgoing synapses  \internal [readonly; units=;]
  int nOutgoing;

  //! Current size  of array (in number of synapses) allocated for outgoing synapses \internal [hidden]
  int nOutgoingAlloc;

};

#endif
