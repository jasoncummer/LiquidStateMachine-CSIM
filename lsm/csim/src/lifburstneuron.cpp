/*! \file lifburstneuron.cpp
**  \brief Implementation of LifNeuron
*/


#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "lifburstneuron.h"
#include "mexnetwork.h"
#include "synapse.h"
#include "csimerror.h"


extern MexNetwork *TheNetwork;

LifBurstNeuron::LifBurstNeuron(void) :
  Cm((float)3e-8), Rm((float)1e6), Vresting ((float)-0.06), Vreset(Vresting),
  Vinit(Vreset), Trefract((float)3e-3), Inoise((float)0.0),
  Iinject((float)0.0)
{
  Vthresh = Vresting+(float)0.015;
  UB = 0.25; 
  uB0 = UB;
  rB0 = 1.0;
  uB = UB;
  rB = 1.0;
  DB = 1.0;
  FB = 0.1;
}

LifBurstNeuron::~LifBurstNeuron(void)
{
}

int LifBurstNeuron::updateInternal(void)
{
  C3 = exp(-DT/FB);
  C4 = exp(-DT/DB);  




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

void LifBurstNeuron::reset(void)
{
  SpikingNeuron::reset();
  Vm             = Vinit;     /* set membrane voltage to its initial value */
  nStepsInRefr   = -1;        /* we are not refractory at the begining     */

  uB = UB;
  uB0 = UB;
}

double LifBurstNeuron::nextstate(void)
{
  // Update 'VBtresh' dependent on UB,FB and DF between spikes 
  
  double isi;
  
  isi = 0;
  if (nSpikes() > 0)
    isi = SimulationTime - spikeTime(nSpikes()-1);

  // printf("Last ISI: %g due to %g, %g at spike %d\n", isi, SimulationTime, spikeTime(nSpikes()-1), nSpikes());
  // printf("Integration constants: %g, %g\n", C3, C4);
  uB = uB*C3+UB*(1-C3);
  rB = 1+rB*C4-C4;
  //rB = 1-(1-rB0)*exp(-isi/DB);
  //uB = UB+(uB0-UB)*exp(-isi/FB);

  VBthresh = Vthresh - 0.5*(Vthresh-Vreset)*(rB*uB-0.50*UB)/UB;
  // VBthresh = Vreset + (Vthresh-Vreset)*(rB*uB-0.5*UB)/UB;
  //VBthresh = Vthresh + 1*(Vthresh-Vreset)*(1-rB);
  Isyn = summationPoint;
  
  if (nStepsInRefr > 0) {
    --nStepsInRefr;
    hasFired = 0;
  } else if ( Vm >= VBthresh ) {
    // Note that the neuron has fired!
    hasFired = 1;
    // calc number of steps how long we are refractory
    nStepsInRefr = (int)(Trefract/DT+0.5);
    
    // update facilitation and depression when spike
    
    uB0 = uB+UB*(1-uB);
    rB0 = rB-rB*uB0;
    rB=rB0;uB=uB0;
    // reset to 'Vreset' */
    //Vm = Vreset+2*(Vthresh-Vreset)*(uB-UB);
    Vm = Vreset;
  } else {
    hasFired = 0;
    // all synapses have added the contributions to summationPoint
    // we add I0
    summationPoint += I0;

    if ( Inoise > 0.0 )
      summationPoint += (normrnd()*Inoise);

    // do the exponential Euler integration step
    Vm = C1*Vm+C2*summationPoint;
  }

  SpikingNeuron::nextstate();

  // clear synaptic input for next time step
  summationPoint = 0;
  return Vm;
}

int LifBurstNeuron::isRefractory(void)
{
  return (nStepsInRefr > 0);
}

