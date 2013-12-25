/*! \file neuron.cpp
**  \brief Implementation of Neuron
*/

#include <stdlib.h>
#include "neuron.h"
#include "synapse.h"
#include "csimerror.h"

// Here are the defines for the shared mem file used for the external
// I/O. This is put here bcs all Ext stuff is derived from Neuron.
const char* rtMemFile="/tmp/rtMemFile";         // name of the shared mem file

#define SYNARRAY_INITIAL 20
#define SYNARRAY_INC     20

// csimClassInfo *Neuron::classInfo;

Neuron::Neuron(void)
{
  nIncoming      = 0; 
  nIncomingAlloc = SYNARRAY_INITIAL;
  incoming       = (Synapse **)malloc(nIncomingAlloc*sizeof(Synapse *));

  nOutgoing      = 0;
  nOutgoingAlloc = SYNARRAY_INITIAL;
  outgoing       = (Synapse **)malloc(nOutgoingAlloc*sizeof(Synapse *));

  summationPoint = 0;

  type = 0;
}

Neuron::~Neuron(void)
{
  if (incoming) free(incoming); incoming = 0;
  if (outgoing) free(outgoing); outgoing = 0;
}

void Neuron::getPre(uint32 *idx)
{
  for(int i=0;i<nIncoming;i++)
    *idx++ = incoming[i]->getId();
}

void Neuron::getPost(uint32 *idx)
{
  for(int i=0;i<nOutgoing;i++)
    *idx++ = outgoing[i]->getId();
}

int Neuron::addIncoming(Advancable *a)
{
  Synapse *syn;
  if ( (syn=dynamic_cast<Synapse *>(a)) ) {
    if ( ++nIncoming > nIncomingAlloc ) {
      /* we have to enlarge the array of outgoing synapses */
      nIncomingAlloc += SYNARRAY_INC;
      incoming = (Synapse **)realloc(incoming,nIncomingAlloc*sizeof(Synapse *));
    }
    incoming[nIncoming-1] = syn;
    return 0;
  } else {
    if (Forceable::addIncoming(a) < 0) {
      TheCsimError.add("Neuron::addIncoming can not use %s %i as incoming element!\n",a->className(),a->getId()); 
      return -1;
    } else
      return 0;
  }
}

int Neuron::addOutgoing(Advancable *a)
{
  Synapse *syn;
  if ( (syn=dynamic_cast<Synapse *>(a)) ) {
    if ( ++nOutgoing > nOutgoingAlloc ) {
      /* we have to enlarge the array of outgoing synapses */
      nOutgoingAlloc += SYNARRAY_INC;
      outgoing = (Synapse **)realloc(outgoing,nOutgoingAlloc*sizeof(Synapse *));
    }
    outgoing[nOutgoing-1] = syn;
    return 0;
  } else {
    TheCsimError.add(" Neuron::addOutgoing: can not use %s %i as outgoing element!\n",a->className(),a->getId()); return -1;
  }
}

