/*! \file dynamicspikingcbsynapse.h
**  \brief Class definition of DynamicSpikingCbSynapse
*/

#ifndef _DYNAMICSPIKINGCBSYNAPSE_H_
#define _DYNAMICSPIKINGCBSYNAPSE_H_

#include <math.h>
#include "dynamicspikingsynapse.h"

//! A dynamic spiking synapse (see Markram et al, 1998)
/** The conductance \f$g(t)\f$ of the synapse is increased by
 **  \f$W\cdot r \cdot u\f$ when a presynaptic spike hits the
 **  synapse and decays exponentially (time constant \f$\tau\f$)
 **  otherwise. \f$u\f$ and \f$r\f$ model the current state of
 **  facilitation and depression. */
class DynamicSpikingCbSynapse : public DynamicSpikingSynapse {

  DO_REGISTERING

public:
  DynamicSpikingCbSynapse(void);

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
