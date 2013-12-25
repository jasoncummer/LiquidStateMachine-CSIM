/*! \file lifneuronsynchan.cpp
**  \brief Implementation of LifNeuronSynchan
*/

#include "randgen.h"
#include "lifneuronsynchan.h"
#include "synapse.h"
#include "csimerror.h"

LifNeuronSynchan::LifNeuronSynchan(void) :
  tau_nmda((float)0.15), tau_ampa((float)0.003), tau_gaba_a((float)0.005), tau_gaba_b((float)0.1),
  E_nmda((float)0), E_ampa((float)0), E_gaba_a((float)-0.07), E_gaba_b((float)-0.07),
  Mg_conc((float)1e-3)
{}

LifNeuronSynchan::~LifNeuronSynchan(void) {}

int LifNeuronSynchan::updateInternal(void)
{
  if ( tau_nmda > 0 ) 
    decay_nmda = exp(-DT/tau_nmda);
  else {
    TheCsimError.add("LofNeuronSynchan::updateInternal: tau_nmda <= 0 !\n"); return -1;
  }
  if ( tau_ampa > 0 ) 
    decay_ampa = exp(-DT/tau_ampa);
  else {
    TheCsimError.add("LofNeuronSynchan::updateInternal: tau_ampa <= 0 !\n"); return -1;
  }
  if ( tau_gaba_a > 0 ) 
    decay_gaba_a = exp(-DT/tau_gaba_a);
  else {
    TheCsimError.add("LofNeuronSynchan::updateInternal: tau_gaba_a <= 0 !\n"); return -1;
  }
  if ( tau_gaba_b > 0 ) 
    decay_gaba_b = exp(-DT/tau_gaba_b);
  else {
    TheCsimError.add("LofNeuronSynchan::updateInternal: tau_gaba_b <= 0 !\n"); return -1;
  }
    
  return LifNeuron::updateInternal();
}

void LifNeuronSynchan::reset(void)
{
  LifNeuron::reset();
  summationPoint_nmda = 0;
  summationPoint_ampa = 0;
  summationPoint_gaba_a = 0;
  summationPoint_gaba_b = 0;
}

double LifNeuronSynchan::nextstate(void)
{
  summationPoint_nmda *= decay_nmda; 
  summationPoint_ampa *= decay_ampa; 
  summationPoint_gaba_a *= decay_gaba_a; 
  summationPoint_gaba_b *= decay_gaba_b;

  summationPoint += summationPoint_nmda*(E_nmda-Vm)/(1+exp(-62*Vm)*Mg_conc/3.57);; 
  summationPoint += summationPoint_ampa*(E_ampa-Vm); 
  summationPoint += summationPoint_gaba_a*(E_gaba_a-Vm); 
  summationPoint += summationPoint_gaba_b*(E_gaba_b-Vm);
 
  return LifNeuron::nextstate();
}
