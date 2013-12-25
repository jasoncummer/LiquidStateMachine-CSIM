/*! \file cbhhouneuron.cpp
**  \brief Implementation of conductance based neurons with Ornstein Uhlenbeck process noise
*/

#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "cbhhouneuron.h"
#include "specific_ion_channels.h"
#include <time.h>

CbHHOuNeuron::CbHHOuNeuron(void)
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
   m  = 0;

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


CbHHOuNeuron::~CbHHOuNeuron(void)
{
  if ( k  ) { delete k;  k=0;  }
  if ( na ) { delete na; na=0; }
  if ( m ) { delete m; m=0; }
}

void CbHHOuNeuron::reset(void)
{

  // reset CbNeuron (important is that first the ion channel conductances are reseted)
  CbNeuron::reset();

  double Gtot = 1/Rm;
  int c;
  for(c=0;c<nChannels;c++) {
    Gtot += (channels[c]->gInfty());
  }
  // printf("CbHHOuNeuron::reset Gtot:%g\n",Gtot);

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
  //  rseed(clock());
  //  printf("CbHHOuNeuron::reset %g\n",normrnd());

  OuInoise = 0;
  OuGnoise = 0;
}


double CbHHOuNeuron::nextstate(void)
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


int CbHHOuNeuron::init(Advancable *a)
{
  //printf("CbHHOuNeuron::init \n");

  bool e=0;
  e = CbNeuron::init(a);

  k  = new KDChannel_Traub91();
  na = new NAChannel_Traub91();
  m = new MChannel_Mainen96orig();

  // printf("CbHHOuNeuron::init %i\n",type);

  if ( k ) {
   e = e || a->addIncoming(k) < 0;
   e = e || k->addIncoming(a) < 0;
   e = e || a->addOutgoing(k) < 0;
   e = e || k->addOutgoing(a) < 0;
  }

  if ( na ) {
   e = e || a->addIncoming(na) < 0;
   e = e || na->addIncoming(a) < 0;
   e = e || a->addOutgoing(na) < 0;
   e = e || na->addOutgoing(a) < 0;
  }

  if ( m ) {
   e = e || a->addIncoming(m) < 0;
   e = e || m->addIncoming(a) < 0;
   e = e || a->addOutgoing(m) < 0;
   e = e || m->addOutgoing(a) < 0;
  }
  
  return e ? -1 : 0;

}

int CbHHOuNeuron::updateInternal(void)
{
  // printf("CbHHOuNeuron::updateInternal \n");

  bool e=0;

  // for inhibitory neurons set m channel to 0
  if ( type == 1 ) { m->Gbar = 0;  }

  e = CbNeuron::updateInternal();

  if ( k ) { e = e || k->updateInternal() < 0; }
  if ( na ) { e = e || na->updateInternal() < 0; }
  if ( m ) { e = e || m->updateInternal() < 0; }

  return e;
}



