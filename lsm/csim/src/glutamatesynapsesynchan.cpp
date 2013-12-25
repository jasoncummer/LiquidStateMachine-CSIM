/*! \file glutamatesynapsesynchan.cpp
**  \brief Implementation of GlutamateSynapseSynchan
*/

#include <math.h>
#include "glutamatesynapsesynchan.h"
#include "spikingneuron.h"
#include "lifneuronsynchan.h"
#include "internalvoltageneuron.h"
#include "csimerror.h"

GlutamateSynapseSynchan::GlutamateSynapseSynchan(void)
{
  fact_nmda  = 0.5; 
  fact_ampa  = 1.0; 
  activeSTDP = 0;
}

//#define POST ((InternalVoltageNeuron *)post)
#define POST ((LifNeuronSynchan *)post)

int GlutamateSynapseSynchan::advance(void) {
  // simply return 0. This method should never be called
  // because we never do something in advance.
  return 0;
}

int GlutamateSynapseSynchan::preSpikeHit(void)  
{
  //printf("pre hit!");
  //printf("steps2cutoff=%i\n",steps2cutoff);
  StdpSynapse::preSpikeHit();
  (*summationPoint_nmda) += W*fact_nmda/POST->decay_nmda;
  (*summationPoint_ampa) += W*fact_ampa/POST->decay_ampa;
  return 0; 
};

int GlutamateSynapseSynchan::addOutgoing(Advancable *a)
{
  LifNeuronSynchan *s = dynamic_cast<LifNeuronSynchan *>(a); // Use a Synchan-Class here!
  if (!s) {
    TheCsimError.add("GlutamateSynapseSynchan::addOutgoing: spiking synapse (idx=%i) postsynaptic element with synchans needed(%s)!\n",
                     this->getId(),a->className());
    return -1;
  }
  summationPoint_nmda = &(s->summationPoint_nmda);
  summationPoint_ampa = &(s->summationPoint_ampa);
  return Synapse::addOutgoing(a);
}

#undef POST
