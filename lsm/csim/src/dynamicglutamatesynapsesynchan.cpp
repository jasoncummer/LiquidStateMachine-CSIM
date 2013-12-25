/*! \file dynamicglutamatesynapsesynchan.cpp
**  \brief Implementation of DynamicGlutamateSynapseSynchan
*/

#include <math.h>
#include "dynamicglutamatesynapsesynchan.h"
#include "stdpsynapse.h"
#include "spikingneuron.h"
#include "lifneuronsynchan.h"

DynamicGlutamateSynapseSynchan::DynamicGlutamateSynapseSynchan(void)
{
  u0 = U = (float)0.4;
  r0 = (float)1.0;
  D  = (float)1.0;
  F  = (float)0.01;
  lastSpike = -1 ;
}

DynamicGlutamateSynapseSynchan::~DynamicGlutamateSynapseSynchan(void)
{
}

void DynamicGlutamateSynapseSynchan::reset(void)
{
  GlutamateSynapseSynchan::reset();
  u     = u0;
  r     = r0;
  lastSpike = -1 ;
}

#define POST ((LifNeuronSynchan *)post)

int DynamicGlutamateSynapseSynchan::preSpikeHit(void)  
{
  //printf("pre hit!");
  //printf("steps2cutoff=%i\n",steps2cutoff);
  StdpSynapse::preSpikeHit();
  if ( lastSpike > 0 ) { 
    double isi = SimulationTime - lastSpike;
    r = 1 + (r*(1-u)-1)*exp(-isi/D);
    u = U + u*(1-U)*exp(-isi/F);
  }
  lastSpike = SimulationTime;
  (*summationPoint_nmda) += (W * u * r)*fact_nmda/POST->decay_nmda;
  (*summationPoint_ampa) += (W * u * r)*fact_ampa/POST->decay_ampa;
  return 0; 
}

#undef POST
