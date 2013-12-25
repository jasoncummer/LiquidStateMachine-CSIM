/*! \file spikinginputneuron.cpp
**  \brief Implementation of SpikingInputNeuron
*/

#include "spikinginputneuron.h"
#include "csimerror.h"


void SpikingInputNeuron::reset(void)
{
  SpikingNeuron::reset();
}

double SpikingInputNeuron::nextstate(void)
{
  if ( (outSpike=hasFired=nextValue(0)) ) {
    saveSpike();
    propagateSpike();
    return 1;
  }
  return 0;
}
