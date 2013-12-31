/*! \file dynamicglutamatesynapse.cpp
**  \brief Implementation of DynamicStdpSynapse
*/

#include <math.h>
#include "dynamicglutamatesynapse.h"
#include "spikingneuron.h"

DynamicGlutamateSynapse::DynamicGlutamateSynapse(void)
{
  u0 = U = (float)0.4;
  r0 = (float)1.0;
  D  = (float)1.0;
  F  = (float)0.01;
  lastSpike = -1 ;
}

DynamicGlutamateSynapse::~DynamicGlutamateSynapse(void)
{
}

void DynamicGlutamateSynapse::reset(void)
{
  GlutamateSynapse::reset();
  psr   = 0.0;
  decay = exp(-DT/tau);
  u     = u0;
  r     = r0;
  lastSpike = -1 ;
}

