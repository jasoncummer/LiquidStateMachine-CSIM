/*! \file staticspikingcbsynapse.h
**  \brief Class definition of StaticSpikingCbSynapse
*/

#ifndef _STATICSPIKINGCBSYNAPSE_H_
#define _STATICSPIKINGCBSYNAPSE_H_

#include "staticspikingsynapse.h"

//! A static spike transmitting synapse (no synaptic dynamics)
/** We call a synapse a \e static synapse if the amplitude of each
evoked conductance change is equal.  Here we implement a conductance
\f$g(t)\f$ of the form \f$g(t)=W\cdot\exp(-t/\tau)\f$ for each spike
which hits the synapse at time \f$t\f$ with an amplitude of \f$W\f$
and a decay time constant of \f$\tau\f$.  The responses of all the
spikes are added up linearly. */
class StaticSpikingCbSynapse : public StaticSpikingSynapse {

  DO_REGISTERING

  public:

  StaticSpikingCbSynapse(void);

  //! Advance the state of the Synapse
  virtual int advance(void);

  //! Outgoing objects must be CbNeurons
  virtual int addOutgoing(Advancable *a);

  //! Equilibrium potential given by the Nernst equation.
  double E;

  private:

  //! Pointer to postsynaptic conductance summation point within the \link SynapseTarget synaptic target \endlink ( Neuron, SynapticChannel, Compartment...) \internal [hidden;]
  double *GSummationPoint;

};

#endif
