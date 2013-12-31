/*! \file analoginputneuron.cpp
**  \brief Implementation of AnalogInputNeuron
*/

#include <stdio.h>
#include "analoginputneuron.h"
#include "csimerror.h"

AnalogInputNeuron::AnalogInputNeuron(void)
{
  Vm = 0;
}

double AnalogInputNeuron::nextstate(void)
{
  return (VmOut=Vm=nextValue(0)+Vresting);
}

void AnalogInputNeuron::reset(void)
{
  AnalogNeuron::reset();
  Vm=Vresting;
}

int AnalogInputNeuron::addIncoming(Advancable *)
{
  TheCsimError.add("AnalogInputNeuron::addIncoming: Do not accept any incoming objects!\n");
  return -1;
}

int AnalogInputNeuron::addOutgoing(Advancable *S)
{
  return AnalogNeuron::addOutgoing(S);
}




