/*! \file spikingsynapse.h
**  \brief Class definition of SpikingSynapse
*/

#ifndef _SPIKINGSYNAPSE_H_
#define _SPIKINGSYNAPSE_H_

#include "synapse.h"

class SpikingSynapse;

typedef int (SpikingSynapse:: *pSpikeHandler)(void); // pointer of spike handler routine

class SpikingSynapse;

#define NEED_TO_ACTIVATE \
  /* since psr = psr*decay in the same time step we undo this first */ \
  /* psr /= decay; */ \
  /* do we need to activate this synapse? */ \
  register bool activate = (steps2cutoff == 0); \
  /* now calc the new cutoff point */ \
  steps2cutoff = (int)(PSR_MULTIPLE_TAU*tau/DT+0.5); \
  return activate; 

//! Base class of all spike transmitting synapses
/** It implements the basic behaviour of spiking synapses: after some
 ** transmission delay the spike from the presynaptic neuron hits the
 ** synapse. If "a
 ** vesicle is released" the method preSpikeHit() is called which
 ** implements the actual effect of the spike.
 **
 ** After \link #PSR_MULTIPLE_TAU PSR_MULTIPLE_TAU \endlink time constants are elapsed the postsynaptic response has practically vanished and the synapse is put into a list of idle synapses and activated again if a spikes hits the synapse. This speeds up simulations with low firing rates considerably.
 */
class SpikingSynapse : public Synapse {
 public:

  // The default constructor
  SpikingSynapse(void);

  // The destructor
  virtual ~SpikingSynapse(void);

  // Advance the state of the Synapse
  virtual int advance(void);

  // Reset to initial condition.
  virtual void reset(void);

  // Update internal variables
  virtual int updateInternal(void);


  //! "Release" probability \f$p\f$ of the synapse 
  //! \internal [units=; range=(0,1)];
  /** To be precise: one should call this the probability that the
   ** synapse does not fail since the term release probability
   ** \f$p_i\f$ usually refers to the probability that an individual
   ** release site of a synapse releases a vesicle . Since we are
   ** modeling one functional connection (i.e. all release sites at
   ** once) the failure probability (i.e. no site releases a vesicle)
   ** is given by \f$p_{failure} = \prod_i (1-p_i)\f$. Hence the
   ** probability \f$p\f$ of releasing at least one vesicle is
   ** \f$p=1-p_{failure}\f$ */
  // float p;

  //!The synaptic time constant \f$\tau\f$ [units=sec; range=(0,100)];
  /** A spike causes a exponential decaying postsynaptic response of
   ** the form \f$\exp(-t/\tau)\f$ */
  float tau;

  inline bool checkForActivation(void);

  //! Called by TheNetwork if a presynaptic spike hits (arrives at) the synapse
  /** A return value of 1 indicates that the synapse is currently not
   ** active (last spike more then \f$5 \tau\f$ in the past) and
   ** TheNetwork has to put this synapse into its list of active
   ** synapses. A return value of 0 indicates the the synapse is
   ** currently active and need not to be put into the list of active
   ** synapses.  */
  virtual int preSpikeHit(void);

  //! Will be called to tell the synapse about its presynaptic neuron
  virtual int addIncoming(Advancable *a);

  //! Will be called to tell the synapse about its postsynaptic neuron
  virtual int addOutgoing(Advancable *a);

 protected:

  int steps2cutoff;
  
  //! \internal factor for exponential decay of the state variable \f$x(t)\f$. [hidden;]
  /** \internal It is initialized to \f$\exp(-dt/\tau)\f$ where \f$dt\f$ is the
      integration time constant. */
  double decay;

};

#endif
