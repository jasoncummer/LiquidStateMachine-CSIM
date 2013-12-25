/*! \file sigmoidalneuron.cpp
**  \brief Implementation of SigmoidalNeuron
*/

#include "sigmoidalneuron.h"
#include "randgen.h"

SigmoidalNeuron::SigmoidalNeuron(void) :
  thresh(0.1), beta(10.0), tau_m(0.03),
  A_max(100.0), I_inject(0.0), Vm_init(0.0)
{
  Vm_init = A_max / 2.0;
}

double SigmoidalNeuron::nextstate(void)
{

  // Here we integrate all
  //   - the postsynaptic currents,
  //   - the noise and the
  //   - injected current

  // Itot = I_inject;
  summationPoint += I_inject;

  if ( Inoise > 0.0 )
    summationPoint += normrnd()*Inoise;

  //for(i=0;i<nIncoming;i++) {
  //  Itot += (incoming[i]->psr);
  //}

  // now let's squash it ... and scale Vm to [0,A_max]
  //Itot = A_max/(1.0+exp(-beta*(Itot-thresh)));
  summationPoint = A_max/(1.0+exp(-beta*(summationPoint-thresh)));

  // do the Euler integration step
  // Vm = C1 * Vm + C2 * Itot;
  VmOut = Vm = C1 * (Vm-Vresting) + C2 * summationPoint + Vresting;
        
  summationPoint = 0;

  return Vm;
}

void SigmoidalNeuron::reset(void)
{
  AnalogNeuron::reset();
  VmOut = Vm = Vm_init;                   // set unit output to its initial value
  if ( tau_m > 0 ) {              // init consts for exp. Euler integration
    C1 = exp(-DT/tau_m);
  } else {
    C1 = 0.0;
  }
  C2 = (1-C1);
}
