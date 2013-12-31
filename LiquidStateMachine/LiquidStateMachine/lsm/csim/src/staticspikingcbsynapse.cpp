/*! \file staticspikingcbsynapse.cpp
**  \brief Implementation of StaticSpikingCbSynapse
*/

#include "staticspikingcbsynapse.h"
#include "cbneuron.h"
#include "csimerror.h"


StaticSpikingCbSynapse::StaticSpikingCbSynapse(void)
{
  E = 0.0;
  GSummationPoint = 0;
}



int StaticSpikingCbSynapse::advance(void) {

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


int StaticSpikingCbSynapse::addOutgoing(Advancable *a)
{
  CbNeuron *cn = dynamic_cast<CbNeuron *>(a);

  if (cn) {
     GSummationPoint = &(cn->GSummationPoint);
  } else {
    TheCsimError.add(" StaticSpikingCbSynapse::addOutgoing: object must be a CbNeuron; which %s is not!\n",a->className());
    return -1;
  }


  // connect post pointer to the postsynatpic object
  return StaticSpikingSynapse::addOutgoing(a);
}

