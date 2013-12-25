/*! \file ifbneuron.cpp
**  \brief Implementation of LifNeuron
*/


#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "ifbneuron.h"
#include "mexnetwork.h"
#include "synapse.h"
#include "csimerror.h"


extern MexNetwork *TheNetwork;

IfbNeuron::IfbNeuron(void) :
  Cm((float)3e-8), Rm((float)1e6), Vresting ((float)-0.065), Vreset(Vresting),
  Vinit(Vreset), Trefract((float)3e-3), Inoise((float)0.0),
  Iinject((float)0.0)
{
  //Vresting = 0.0;
  Vthresh = Vresting+(float)0.030;
  h=0;
  Vh= (float)-0.06;
  tau_m = 20e-3;
  tau_p = 100e-3;
  gh = (float)2;
  Vreset = (float)-0.05;
}

IfbNeuron::~IfbNeuron(void)
{
}

int IfbNeuron::updateInternal(void)
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

void IfbNeuron::reset(void)
{
  SpikingNeuron::reset();
  Vm             = Vinit;     /* set membrane voltage to its initial value */
  nStepsInRefr   = -1;        /* we are not refractory at the begining     */
}

double IfbNeuron::nextstate(void)
{
  // update h
  if (Vm>Vh)
    h = h-DT*h/tau_m;
  else
    h = h+DT*(1-h)/tau_p;



  Isyn = summationPoint;
  if (nStepsInRefr > 0) {
    --nStepsInRefr;
    hasFired = 0;
  } else if ( Vm >= Vthresh ) {
    // Note that the neuron has fired!
    hasFired = 1;
    // calc number of steps how long we are refractory
    nStepsInRefr = (int)(Trefract/DT+0.5);
    // reset to 'Vreset' */
    Vm = Vreset;
  } else {
    hasFired = 0;
    // all synapses have added the contributions to summationPoint
    // we add I0
    summationPoint += I0;

    if ( Inoise > 0.0 )
      summationPoint += (normrnd()*Inoise);

    // do the exponential Euler integration step

   if(Vm>Vh) Vm = C1*Vm+C2*summationPoint+gh*C2*h;
   else 
   Vm = C1*Vm+C2*summationPoint; 
  }

  SpikingNeuron::nextstate();

  // clear synaptic input for next time step
  summationPoint = 0;
  return Vm;
}

int IfbNeuron::isRefractory(void)
{
  return (nStepsInRefr > 0);
}

