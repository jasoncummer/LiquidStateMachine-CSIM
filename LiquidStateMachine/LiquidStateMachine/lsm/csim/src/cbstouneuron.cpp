/*! \file cbstouneuron.cpp
**  \brief Implementation of conductance based neurons with Ornstein Uhlenbeck process noise
*/

#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "cbstouneuron.h"
#include <time.h>

CbStOuNeuron::CbStOuNeuron(void)
{
   ge = 0;
   gi = 0;
   ge0 = 0;
   gi0 = 0;
   sig_e = 0;
   sig_i = 0;

   tau_e = 3e-3;
   tau_i = 6e-3;
   Ee = 40e-3;
   Ei = 0e-3;

   Ae = 0;
   Ai = 0;
   Ce = 0;
   Ci = 0;
   De = 0;
   Di = 0;

   OuInoise = 0;
   OuGnoise = 0;
}


void CbStOuNeuron::reset(void)
{
  // reset CbNeuronSt (important is that first the ion channel conductances are reseted)
  CbNeuronSt::reset();

  double Gtot = 1/Rm;
  int c;
  for(c=0;c<nChannels;c++) {
    Gtot += (channels[c]->gInfty());
  }
  // printf("CbStOuNeuron::reset Gtot:%g\n",Gtot);

  // set the conductance distribution parameters relative to the leak conductance

  /*
  ge0 = 0.7*Gtot;
  gi0 = 5*ge0;

  sig_e = ge0*0.4;
  sig_i = gi0*0.4;
  */

  // initialize running conductance variables
  ge = ge0;
  gi = gi0;

  // set parameters for the Ornstein Uhlenbeck process
  De = pow(sig_e,2) * 2/tau_e;
  Di = pow(sig_i,2) * 2/tau_i;

  Ae = sqrt( De*tau_e/2 * (1-exp(-2*DT/tau_e)));
  Ai = sqrt( Di*tau_i/2 * (1-exp(-2*DT/tau_i)));

  Ce = exp(-DT/tau_e);
  Ci = exp(-DT/tau_i);

  // set seed of random number generator
  // rseed(clock());

  OuInoise = 0;
  OuGnoise = 0;
}


double CbStOuNeuron::nextstate(void)
{
  // Synaptic background noise current: Ornstein Uhlenbeck process


  double IsynBuffer = summationPoint;	// store to know true synaptic input
  double GsynBuffer = GSummationPoint;

  ge = fabs( ge0 + (ge - ge0) * Ce + Ae*normrnd() );
  gi = fabs( gi0 + (gi - gi0) * Ci + Ai*normrnd() );

  OuInoise = ge * Ee + gi * Ei;	// store noise for external recordings
  OuGnoise = ge + gi;

  summationPoint += OuInoise;	// add to ensure correct contribution to Vm update
  GSummationPoint += OuGnoise;

  double VmBuffer = CbNeuronSt::nextstate();

  Isyn = IsynBuffer;
  Gsyn = GsynBuffer;

  return VmBuffer;
}
