/*! \file lifneuron.cpp
**  \brief Implementation of LifNeuron
*/

#include "randgen.h"
#include "lifneuron.h"
#include "synapse.h"
#include "csimerror.h"

LifNeuron::LifNeuron(void) :
  Cm((float)3e-8), Rm((float)1e6), Vresting ((float)-0.06), Vreset(Vresting),
  Vinit(Vreset), Trefract((float)3e-3), Inoise((float)0.0),
  Iinject((float)0.0)
{
  Vthresh = Vresting+(float)0.015;
}

LifNeuron::~LifNeuron(void)
{
}

int LifNeuron::updateInternal(void)
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

void LifNeuron::reset(void)
{
  SpikingNeuron::reset();
  Vm             = Vinit;     /* set membrane voltage to its initial value */
  nStepsInRefr   = -1;        /* we are not refractory at the begining     */
  summationPoint = 0;
}

double LifNeuron::nextstate(void)
{
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
    Vm = C1*Vm+C2*summationPoint;
  }

  SpikingNeuron::nextstate();

  // clear synaptic input for next time step
  summationPoint = 0;
  return Vm;
}

int LifNeuron::isRefractory(void)
{
  return (nStepsInRefr > 0);
}
