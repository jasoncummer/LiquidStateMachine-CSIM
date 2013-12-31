/*! \file staticanalogcbsynapse.cpp
**  \brief Implementation of conductance based StaticAnalogSynapse
*/

#include "staticanalogcbsynapse.h"
#include "cbneuron.h"
#include "randgen.h"
#include "stdio.h"
#include "csimerror.h"

StaticAnalogCbSynapse::StaticAnalogCbSynapse(void)
{
  psr = psi = 0;
  W = 1;
  E = 0.0;
  GSummationPoint = 0;
}

void StaticAnalogCbSynapse::reset(void)
{
  advance();
}

int StaticAnalogCbSynapse::advance(void)
{
  double gsyn = psi * W;

  if ( Inoise > 0.0 )
    psr = gsyn * E + normrnd() * Inoise;
  else
    psr = gsyn * E;

  (*summationPoint) += psr;
  (*GSummationPoint) += gsyn;
  
  return 1;
}


int StaticAnalogCbSynapse::addOutgoing(Advancable *a)
{
  CbNeuron *cn = dynamic_cast<CbNeuron *>(a);

  if (cn) {
    GSummationPoint = &(cn->GSummationPoint);
  } else {
    TheCsimError.add(" StaticAnalogCbSynapse::addOutgoing: object must be a CbNeuron; which %s is not!\n",a->className());
    return -1;
  }

  // connect post pointer to the postsynatpic object
  return Synapse::addOutgoing(a);
}



