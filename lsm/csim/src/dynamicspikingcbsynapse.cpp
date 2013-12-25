/*! \file dynamicspikingcbsynapse.cpp
**  \brief Implementation of DynamicSpikingCbSynapse
*/

#include "dynamicspikingcbsynapse.h"
#include "cbneuron.h"
#include "csimerror.h"


DynamicSpikingCbSynapse::DynamicSpikingCbSynapse(void)
{
  E = 0.0;
  GSummationPoint = 0;
}



int DynamicSpikingCbSynapse::advance(void) {

  psr *= decay;
  (*summationPoint) += psr * E;
  (*GSummationPoint) += psr;

  if (--steps2cutoff > 0) {
    return 1;
  } else {
    psr = 0;
    return 0;
  }
}


int DynamicSpikingCbSynapse::addOutgoing(Advancable *a)
{
  CbNeuron *cn = dynamic_cast<CbNeuron *>(a);

  if (cn) {
     GSummationPoint = &(cn->GSummationPoint);
  } else {
    TheCsimError.add(" DynamicSpikingCbSynapse::addOutgoing: object must be a CbNeuron; which %s is not!\n",a->className());
    return -1;
  }


  // connect post pointer to the postsynatpic object
  return DynamicSpikingSynapse::addOutgoing(a);
}
