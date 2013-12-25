/*! \file spikingsynapse.cpp
**  \brief Implementation of SpikingSynapse
*/

#include "randgen.h"
#include "spikingneuron.h"
#include "spikingsynapse.h"
#include "csimerror.h"
#include "lifneuron.h"

SpikingSynapse::SpikingSynapse(void)
{
  W     = (float)1.0e-9;
  //p     = (float)1.0; // we make a relaible synapse
  tau   = (float)3e-3;
  steps2cutoff = 0;
}

SpikingSynapse::~SpikingSynapse(void)
{
}

int SpikingSynapse::updateInternal(void) {
  if ( tau > 0 ) 
    decay = exp(-DT/tau);
  else {
    TheCsimError.add("SpikingSynapse::updateInternal: tau <= 0 !\n"); return -1;
  }
  return 0;
}

void SpikingSynapse::reset(void)
{
  psr = 0;
  steps2cutoff = 0;
}

int SpikingSynapse::advance(void) {
  psr *= decay;
  (*summationPoint) += psr;
  if (--steps2cutoff > 0) {
    return 1;
  } else {
    psr = 0;
    return 0;
  }
}

int SpikingSynapse::preSpikeHit(void)  
{
  return checkForActivation(); 
};

inline bool SpikingSynapse::checkForActivation(void) {
  /* since psr = psr*decay in the same time step we undo this first */
  // psr /= decay;
  /* do we need to activate this synapse? */
  bool activate = (steps2cutoff == 0);
  /* now calc the new cutoff point */
  steps2cutoff = (int)(PSR_MULTIPLE_TAU*tau/DT+0.5);
  return activate; 
}

int SpikingSynapse::addIncoming(Advancable *a)
{
  SpikingNeuron *s = dynamic_cast<SpikingNeuron *>(a);
  if (!s) {
    TheCsimError.add("SpikingSynapse::addIncoming: spiking synapse (idx=%i) has non-spiking presynaptic element (%s)!\n",
                     this->getId(),a->className());
    return -1;
  }
  return Synapse::addIncoming(a);
}

int SpikingSynapse::addOutgoing(Advancable *a)
{
  return Synapse::addOutgoing(a);
}
