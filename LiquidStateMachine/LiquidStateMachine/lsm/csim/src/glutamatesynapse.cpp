/*! \file glutamatesynapse.cpp
**  \brief Implementation of GlutamateSynapse
*/

#include <math.h>
#include "glutamatesynapse.h"
#include "spikingneuron.h"
#include "lifneuron.h"
#include "internalvoltageneuron.h"
#include "csimerror.h"

GlutamateSynapse::GlutamateSynapse(void)
{
  tau_nmda   = (float)0.15;
  fact_nmda  = 0.5; 
  fact_ampa  = 1.0; 
  E_nmda = 0;
  E_ampa = 0;
  Mg_conc = 1e-3;
  activeSTDP = 0;
}

//#define POST ((InternalVoltageNeuron *)post)
#define POST ((LifNeuron *)post)

int GlutamateSynapse::updateInternal(void) {
  if ( tau_nmda > 0 ) 
    decay_nmda = exp(-DT/tau_nmda);
  else {
    TheCsimError.add("GlutamateSynapse::updateInternal: tau_nmda <= 0 !\n"); return -1;
  }
  return StdpSynapse::updateInternal();
}

void GlutamateSynapse::reset(void)
{
  StdpSynapse::reset();
  decay_nmda = exp(-DT/tau_nmda);
  psr_nmda=0.0;
}

int GlutamateSynapse::advance(void) {
  psr_nmda *= decay_nmda; 
  double Vm = POST->Vm;
  
  psr *= decay;
  //printf("volg=%f(%f)",E_ampa-Vm,(float)Vm);
  (*summationPoint) += psr*(E_ampa-Vm) + psr_nmda*(E_nmda-Vm)/(1+exp(-62*Vm)*Mg_conc/3.57);
  if (--steps2cutoff > 0) {
    return 1;
  } else {
    psr = 0;
    return 0;
  }
}

int GlutamateSynapse::preSpikeHit(void)  
{
  //printf("pre hit!");
  //printf("steps2cutoff=%i\n",steps2cutoff);
  register bool activate = (steps2cutoff == 0);
  StdpSynapse::preSpikeHit();
  steps2cutoff = (int)(PSR_MULTIPLE_TAU*tau_nmda/DT+0.5);
  return activate; 
};

inline bool GlutamateSynapse::checkForActivation(void) {
  /* since psr = psr*decay in the same time step we undo this first */
  // psr /= decay;
  /* do we need to activate this synapse? */
  bool activate = (steps2cutoff == 0);
  /* now calc the new cutoff point */
  steps2cutoff = (int)(PSR_MULTIPLE_TAU*tau_nmda/DT+0.5);
  return activate; 
}

int GlutamateSynapse::addOutgoing(Advancable *a)
{
  InternalVoltageNeuron *s = dynamic_cast<InternalVoltageNeuron *>(a); // Use a Vm-Class here!
  if (!s) {
    TheCsimError.add("GlutamateSynapse::addOutgoing: spiking synapse (idx=%i) postsynaptic element with internal voltage needed(%s)!\n",
                     this->getId(),a->className());
    return -1;
  }
  return Synapse::addOutgoing(a);
}

#undef POST
