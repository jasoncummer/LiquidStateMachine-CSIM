/*! \file izhi_neuron.cpp
**  \brief Implementation of Izhi_Neuron
*/


#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "lifneuron.h"
#include "mexnetwork.h"
#include "synapse.h"
#include "csimerror.h"
#include "izhi_neuron.h"

extern MexNetwork *TheNetwork;

Izhi_Neuron::Izhi_Neuron(void) :
  Cm((float)3e-8), Rm((float)1e6), Vresting ((float)-0.06), Vreset(Vresting),
  Vinit(Vreset), Trefract((float)3e-3), Inoise((float)0.0),
  Iinject((float)0.0)
{
  a= 0.0035;
  b= 0.2;
  c= -50;
  d= 2;
  Vthresh = 30;//in millivolt
  u=0;ub=0;Vint=1000*Vinit;
  nStepsInRefr=-1;
}

Izhi_Neuron::~Izhi_Neuron(void)
{
}

int Izhi_Neuron::updateInternal(void)
{

  SpikingNeuron::updateInternal();

  double tau = Cm*Rm;          /* the membrane time constant                          */
  if ( tau > 0 ) {             /* init consts C1,C2 for exponential Euler integration */
    C1 = exp(-DT/tau);
    C2 = Rm*(1-C1);
  } else {
    C1 = 0.0;
    C2 = Rm;
  }
  if ( Rm > 0 )
    I0 =  Iinject + Vresting/Rm;
  else {
    TheCsimError.add("LifNeuron::fieldChangeNotify: Rm <= 0!\n"); return -1;
  }
  return 0;
}

void Izhi_Neuron::reset(void)
{
  SpikingNeuron::reset();
  Vm             = Vinit;     /* set membrane voltage to its initial value */
  nStepsInRefr   = -1;        /* we are not refractory at the begining     */
}

double Izhi_Neuron::nextstate(void)
{
  
  Isyn = summationPoint;
 if ( Vint >= Vthresh ) {
    // Note that the neuron has fired!
    hasFired = 1;
    
    // reset to 'Vreset' */
    Vint = c;
    ub = ub+d;
  } else {
    hasFired = 0;
    // all synapses have added the contributions to summationPoint
    // we add I0
    summationPoint += I0;

    if ( Inoise > 0.0 )
      summationPoint += (normrnd()*Inoise);

    // do the Euler integration step
    
    Vb=Vint+DT*1000*(0.04*Vint*Vint+5*Vint+140-ub)+6000*C2*summationPoint;
    ub=ub+DT*1000*a*(b*Vint-ub);
    Vm=Vb*0.001;
    Vint=Vb;
    u=0.001*ub;

  }

  SpikingNeuron::nextstate();

  // clear synaptic input for next time step
  summationPoint = 0;
  return Vm;
}

int Izhi_Neuron::isRefractory(void)
{
  return (nStepsInRefr > 0);
}







