/*! \file synapse.cpp
**  \brief Implementation of Synapse
*/

#include "synapse.h"
#include "csimerror.h"
#include "neuron.h"

Synapse::Synapse(void)
{
  pre = post = 0;
  delay = 0;
  W     = 1;
}

int Synapse::addOutgoing(Advancable *a)
{
  SynapseTarget *t = dynamic_cast<SynapseTarget *>(a);
  if ( t ) {
    summationPoint = &(t->summationPoint); post = a; return 0;
  } else {
    TheCsimError.add(" Synapse::addOutgoing: object must be a synaptic target; which %s is not!\n",a->className());
    return -1;
  }
  return 0;
}

int Synapse::addIncoming(Advancable *a)
{ 
  pre = a;
  return 0;
}

