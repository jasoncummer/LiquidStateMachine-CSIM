/*! \file hh_neuron.cpp 
**  \brief Implementation of HHNeuron
*/

#include "hh_neuron.h"
#include "hh_squid_channels.h"

HHNeuron::HHNeuron(void)
{
  Rm = 424.4135437*1e3;     // Ohm
  Cm = 0.007853974588*1e-6; // Farad
  Vresting = -0.05;         // Volt

  Vreset=Vresting;

  Vinit=Vresting;
  Trefract=0.005;
  Inoise=0.0;
  Iinject=0.0;
  Vthresh=0.0;
  doReset=0;

  k  = 0;
  na = 0;
}

int HHNeuron::init(Advancable *a)
{


  k  = new HH_K_Channel(0.2827430964*1e-3,-11.99979305*1e-3+Vresting);
  na = new HH_Na_Channel(0.9424769878*1e-3,0.1150009537+Vresting);

  bool e = 0;

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

int HHNeuron::updateInternal(void)
{
  bool e=0;
  e = e || k->updateInternal() < 0;
  e = e || na->updateInternal() < 0;
  return e;
}

HHNeuron::~HHNeuron(void)
{
  if ( k  ) { delete k;  k=0;  }
  if ( na ) { delete na; na=0; }
}
