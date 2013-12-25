/*! \file dynamicspikingsynapse.h
**  \brief Class definition of DynamicSpikingSynapse
*/

#ifndef _DYNAMICSPIKINGSYNAPSE_H_
#define _DYNAMICSPIKINGSYNAPSE_H_

#include <math.h>
#include "spikingsynapse.h"
#include "spikingneuron.h"

//! A dynamic spiking synapse (see Markram et al, 1998)
/** The time varying state \f$x(t)\f$ of the synapse is increased by
 **  \f$W\cdot r \cdot u\f$ when a presynaptic spike hits the
 **  synapse and decays exponentially (time constant \f$\tau\f$)
 **  otherwise. \f$u\f$ and \f$r\f$ model the current state of
 **  facilitation and depression. */
class DynamicSpikingSynapse : public SpikingSynapse  {
  
  DO_REGISTERING

public:
 
#include "dynamicsynapse.h"
 
public:
  DynamicSpikingSynapse(void);
  ~DynamicSpikingSynapse(void);
  
  virtual void reset(void);
  
  virtual int preSpikeHit(void) {
    // Change the PSR (defined in dynamicsynapse.h)
    DYNAMIC_PSR_CHANGE;

    // Check for activation(defined in spikingsynapse.h)
    NEED_TO_ACTIVATE;
  };
  
};

#endif
