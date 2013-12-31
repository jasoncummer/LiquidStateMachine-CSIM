/*! \file dynamicstdpsynapse.cpp
**  \brief Implementation of DynamicStdpSynapse
*/

#include <math.h>
#include "dynamicstdpsynapse.h"
#include "spikingneuron.h"

DynamicStdpSynapse::DynamicStdpSynapse(void)
{
  u0 = U = (float)0.4;
  r0 = (float)1.0;
  D  = (float)1.0;
  F  = (float)0.01;
  lastSpike = -1 ;
}

DynamicStdpSynapse::~DynamicStdpSynapse(void)
{
}

void DynamicStdpSynapse::reset(void)
{
  StdpSynapse::reset();
  psr   = 0.0;
  decay = exp(-DT/tau);
  u     = u0;
  r     = r0;
  lastSpike = -1 ;
}


