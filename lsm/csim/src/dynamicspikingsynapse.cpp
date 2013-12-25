/*! \file dynamicspikingsynapse.cpp
**  \brief Implementation of DynamicSpikingSynapse
*/

#include "dynamicspikingsynapse.h"
#include "spikingneuron.h"


DynamicSpikingSynapse::DynamicSpikingSynapse(void)
{
  u0 = U = (float)0.4;
  r0 = (float)1.0;
  D  = (float)1.0;
  F  = (float)0.01;
  lastSpike = -1 ;
}

DynamicSpikingSynapse::~DynamicSpikingSynapse(void)
{
}

void DynamicSpikingSynapse::reset(void)
{
  SpikingSynapse::reset();
  psr   = 0.0;
  decay = exp(-DT/tau);
  u     = u0;
  r     = r0;
  lastSpike = -1 ;
}


