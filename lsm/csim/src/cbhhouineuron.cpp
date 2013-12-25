/*! \file cbhhouineuron.cpp
**  \brief Implementation of conductance based neurons with Ornstein Uhlenbeck process noise
*/

#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "cbhhouineuron.h"
#include "specific_ion_channels.h"
#include <time.h>

CbHHOuINeuron::CbHHOuINeuron(void)
{
   Rm = 100e6;     // Ohm
   Cm = 3e-10; // Farad

   Vresting = 0;
   Vreset=Vresting;
   Vinit=Vresting;

   Vthresh=0.03;
   doReset=0;

//   nummethod = 0;
   nummethod = 1;
 
   k  = 0;
   na = 0;

   // parameters for the OU process

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


CbHHOuINeuron::~CbHHOuINeuron(void)
{
  if ( k  ) { delete k;  k=0;  }
  if ( na ) { delete na; na=0; }
}

void CbHHOuINeuron::reset(void)
{

  // reset CbNeuron (important is that first the ion channel conductances are reseted)
  CbNeuron::reset();

  double Gtot = 1/Rm;
  int c;
  for(c=0;c<nChannels;c++) {
    Gtot += (channels[c]->gInfty());
  }
  // printf("CbHHOuINeuron::reset Gtot:%g\n",Gtot);

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


double CbHHOuINeuron::nextstate(void)
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

  double VmBuffer = CbNeuron::nextstate();

  Isyn = IsynBuffer;
  Gsyn = GsynBuffer;

  return VmBuffer;
}


int CbHHOuINeuron::init(Advancable *a)
{
  //printf("CbHHOuINeuron::init \n");

  bool e=0;
  e = CbNeuron::init(a);

  k  = new KDChannel_Traub91();
  na = new NAChannel_Traub91();

  e = e || a->addIncoming(k) < 0;
  e = e || k->addIncoming(a) < 0;
  e = e || a->addOutgoing(k) < 0;
  e = e || k->addOutgoing(a) < 0;

  e = e || a->addIncoming(na) < 0;
  e = e || na->addIncoming(a) < 0;
  e = e || a->addOutgoing(na) < 0;
  e = e || na->addOutgoing(a) < 0;
  
  return e ? -1 : 0;

}

int CbHHOuINeuron::updateInternal(void)
{
  // printf("CbHHOuINeuron::updateInternal \n");

  bool e=0;
  e = CbNeuron::updateInternal();

  e = e || k->updateInternal() < 0;
  e = e || na->updateInternal() < 0;

  return e;

}



