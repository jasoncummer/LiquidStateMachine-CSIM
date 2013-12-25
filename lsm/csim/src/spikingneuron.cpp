/*! \file spikingneuron.cpp
**  \brief Implementation of SpikingNeuron
*/

#include "spikingneuron.h"
#include "spikingsynapse.h"
#include "stdpsynapse.h"
#include "network.h"

#define SPIKEARRAY_INITIAL 100

float zero_delay = 0.0;

SpikingNeuron::SpikingNeuron(void)
{
  nEmittedSpikes = 0;
  nSpikesAlloc = SPIKEARRAY_INITIAL;
  Spikes = (double *)malloc(nSpikesAlloc*sizeof(double));
}

SpikingNeuron::~SpikingNeuron(void)
{
  if (Spikes) { free(Spikes); Spikes=0; }
  SpikeReceiver.destroy();
}

void SpikingNeuron::reset(void)
{
  Neuron::reset();
  nEmittedSpikes = 0;
}

int SpikingNeuron::addIncoming(Advancable *a)
{
  // if we have a synapse where we have to send the spike back we add it to a seperate list!
  StdpSynapse *s = dynamic_cast<StdpSynapse *>(a);
  if ( s ) {
    //    postSpikeSynapsesList.add(s);
    SpikeReceiver.add(new SpikeDest((SpikingSynapse *)s,(pSpikeHandler)(&StdpSynapse::postSpikeHit),&(s->back_delay)));
  }

  // and it is also added to the list of the usual incoming synapses
  return  Neuron::addIncoming(a);
}

int  SpikingNeuron::addOutgoing(Advancable *a)
{
  SpikingSynapse *s = dynamic_cast<SpikingSynapse *>(a);
  if (s != NULL) {    
    SpikeReceiver.add(new SpikeDest(s,&SpikingSynapse::preSpikeHit,&(s->delay)));
    return Neuron::addOutgoing(a);
  } else
    return -1;
}

void SpikingNeuron::saveSpike(void)
{
  /* append spike to spike list */
  if ( nEmittedSpikes >= nSpikesAlloc ) {
    /* we ran out of space storing the spikes ... */
    nSpikesAlloc *= 2;
    Spikes = (double *)realloc(Spikes,nSpikesAlloc*sizeof(double));
  }
  Spikes[nEmittedSpikes++] = SimulationTime;      /* save the spike */
}

void SpikingNeuron::propagateSpike(void)
{
  for(unsigned s=0;s<SpikeReceiver.n;s++)
    TheCurrentNetwork()->ScheduleSpike(SpikeReceiver.elements[s]->synapse,
                              SpikeReceiver.elements[s]->spikeHandler,*(SpikeReceiver.elements[s]->delay));

}
