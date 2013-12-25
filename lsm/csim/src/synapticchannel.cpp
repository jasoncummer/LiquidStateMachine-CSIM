#include <stdlib.h>
#include "synapticchannel.h"
#include "synapse.h"


SynapticChannel::SynapticChannel(void)
{
  nSynapses = lSynapses = 0;
  synapses=0;
}

SynapticChannel::~SynapticChannel()
{
  if (synapses) free(synapses); synapses = 0;
}

void SynapticChannel::reset(void)
{
  advance();
}

int SynapticChannel::advance(void)
{
  g = 0;
  for(int i=0;i<nSynapses;i++)
    g += (synapses[i]->psr);

  g *= Gbar;
  return 1;
}

void SynapticChannel::addSynapse(Synapse *s) 
{
  if ( ++nSynapses > lSynapses ) {
    lSynapses += 20;
    synapses   = (Synapse **)realloc(synapses,lSynapses*sizeof(Synapse *));
  }
  synapses[nSynapses-1] = s;
}

int SynapticChannel::addIncoming(Advancable *Incoming)
{
  Synapse *s=dynamic_cast<Synapse *>(Incoming);
  if ( s ) {
    addSynapse(s); return 1;
  } else {
    return IonChannel::addIncoming(Incoming);
  }
}

int SynapticChannel::addOutgoing(Advancable *Outgoing)
{
  return IonChannel::addOutgoing(Outgoing);
}
