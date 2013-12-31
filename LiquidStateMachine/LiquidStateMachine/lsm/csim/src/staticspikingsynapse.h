/*! \file staticspikingsynapse.h
**  \brief Class definition of StaticSpikingSynapse
*/

#ifndef _STATICSPIKINGSYNAPSE_H_
#define _STATICSPIKINGSYNAPSE_H_

#include "globaldefinitions.h"
#include "spikingsynapse.h"

//! A static spike transmitting synapse (no synaptic dynamics)
/** We call a synapse a \e static synapse if the amplitude of each
postsynaptic response is equal.  Here we implement a synaptic response
\f$x(t)\f$ of the form \f$x(t)=W\cdot\exp(-t/\tau)\f$ for each spike
which hits the synapse at time \f$t\f$ with an amplitude of \f$W\f$
and a decay time constant of \f$\tau\f$.  The responses of all the
spikes are added up linearly. */
class StaticSpikingSynapse : public SpikingSynapse {

  DO_REGISTERING 

  virtual int preSpikeHit(void) {

    // increase the response
    psr += (W/decay);

    // Check for activation(defined in spikingsynapse.h)
    NEED_TO_ACTIVATE

   }
  
};

#endif
