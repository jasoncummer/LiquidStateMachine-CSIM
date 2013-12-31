/*! \file analogneuron.cpp
**  \brief Implementation of AnalogNeuron
*/

#include "analogneuron.h"
#include "csimerror.h"

//! Shared Memory constants and variables
int nSharedMemUse = 0;            // sort of a semaphore showing the use of shared mem
double* sharedData;               // pointer to shared mem
int memID;                        // file ID of shared mem file

AnalogNeuron::AnalogNeuron(void) : Vm(0.0), Inoise(0.0)
{
  delayQueue = new Queue;
  delayQueue->size  = 0;
  delayQueue->entry = 0;
  delayQueue->current = 0;
  Vresting = 0.0;
}

AnalogNeuron::~AnalogNeuron()
{
  if (delayQueue) {
    if ( delayQueue->entry ) free(delayQueue->entry); delayQueue->entry = 0;
    delete delayQueue;
  }
  delayQueue = 0;
}

void AnalogNeuron::force(double theForcedValue)
{
  VmOut = theForcedValue;
}

void AnalogNeuron::reset(void)
{
  int i;
  unsigned j;
  double maxDelay;
  AnalogSynapse* tempSyn;

  Neuron::reset();

  // first search through all synapses on my axon (->outgoing)
  // and find the longest delay. this delay will determine
  // the length of the queue.
  maxDelay = 0.0;
  for (i=0;i<nOutgoing;i++) {
    tempSyn = (AnalogSynapse *)(outgoing[i]);
    if ( tempSyn ) {
      if ( tempSyn->delay > maxDelay )
	maxDelay = tempSyn->delay;
      // also determine the correct delay pointers of each post-synapse
      // and store it's value for faster computation
      tempSyn->delayIndex = ((int)(tempSyn->delay / DT)) + 1;
    } else {
      TheCsimError.add("AnalogNeuron::reset non analog outgoing synapses!!\n"); return;
    }
  }
  delayQueue->size  = ((int)(maxDelay / DT)) + 1;
  if ( delayQueue->entry ) free(delayQueue->entry); delayQueue->entry = 0;
  delayQueue->entry = (double *)malloc(delayQueue->size*sizeof(double));
  delayQueue->current = 1;
  for (j=0;j<delayQueue->size;j++) delayQueue->entry[j] = 0.0;

  VmOut = Vm = Vresting;
  output();
}

void AnalogNeuron::output(void)
{
  int i;
  AnalogSynapse* tempSyn;

  // put the current Vm into the queue
  putInQueue(VmOut);
  // search through all synapses on my axon (->outgoing)
  // and promote the correctly delayed output
  for (i=0;i<nOutgoing;i++) {
    tempSyn = (AnalogSynapse *)(outgoing[i]);
    tempSyn->psi = getFromQueue(tempSyn->delayIndex);
  }
}

int AnalogNeuron::addIncoming(Advancable *S)
{
  return Neuron::addIncoming(S);
}

int AnalogNeuron::addOutgoing(Advancable *S)
{

 /* DOESNT WORK WITH MEMBRANEPATCH CONCEPT
  IonChannel *ionCh = dynamic_cast<IonChannel *>(S);
  if (ionCh != NULL) {
    return 0;
  }
 */ 

  // dynamic_cast returns NULL if S is not derived from AnalogSynapseor ActiveChannel
  AnalogSynapse *anaSyn = dynamic_cast<AnalogSynapse *>(S);
  if (anaSyn != NULL)
    return Neuron::addOutgoing(S);
  else {
    TheCsimError.add(" AnalogNeuron::addOutgoing: can not use %s %i as outgoing element!\n",
		     S->className(),S->getId());
    return -1;
  };
}

double AnalogNeuron::getFromQueue(int delayIndex) {
  int myIndex;

  myIndex = delayQueue->current-delayIndex;
  // since the size of the queue depends on the maximum delay
  // of all post-synapses we have to add the size at maximum
  // once to the required index
  if ( myIndex < 0 ) myIndex += delayQueue->size;
  return ( delayQueue->entry[myIndex] );
}


/** \attention With every call to putInQueue a time step of dt is
 ** assumed. So be careful to call this only once during one time step
 ** step! */
void AnalogNeuron::putInQueue(double theValue) {
  if (delayQueue->current < delayQueue->size)
    delayQueue->current++;
  else
    delayQueue->current=1;

  delayQueue->entry[delayQueue->current-1] = theValue;
}
