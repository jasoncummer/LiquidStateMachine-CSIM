#include "csiminputclass.h"
#include "csimerror.h"

void csimInputClass::addChannel(csimInputChannel *c) {
  channel.add(c);
  channelIdChecked=0;
}

void csimInputClass::clearInputChannels(void) {
  channel.clear();
}

csimInputChannel* csimInputClass::getChannel(uint32 i) {
  if ( i<channel.n ) {
    //    printf("csimInputClass::getChannel %i\n",i);
    return channel.elements[i];
  } else {
    if ( !channelIdChecked ) {
      // TheCsimError.add("csimInputClass::getChannel: no such channel index (%i)!\n",i);
      csimPrintf("CSIM WARNING: csimInputClass::getChannel: no such channel index (%i)!\n",i);
      channelIdChecked=1;
    }
    return 0;
    //printf("FATAL: csimInputClass::getChannel: no such channel index (%i)!\n",i);
    //printf("Exitting!\n");
    //exit(-1);
  }
}


/********************************************************************************
 ** SPIKING INPUTS
 *******************************************************************************/

int csimSpikingInputClass::addInputChannel(csimInputChannel *i)
{
  if ( i->dt != -1 ) {
    TheCsimError.add("csimSpikingInputClass::addChannel: no spiking input!\n");
    return -1;
  }

  //  printf("csimSpikingInputClass::addInputChannel %i\n",i->idx[0]);

  // find next spike after current SimulationTime
  for(i->next=0; i->next<i->length; (i->next)++ )
    if ( i->data[i->next] > SimulationTime ) break;

  i->lastSpikeTime = -1;

  csimInputClass::addChannel(i);

  return 0;
}

int csimSpikingInputClass::nextValue(int i) {

  csimInputChannel *s;

  if ( (s=getChannel(i)) != 0 ) {
    //    printf("csimSpikingInputClass::nextValue %g %i\n",s->lastSpikeTime,s->next);
    // Check whether we are at a new simulation step and there was a
    // spike in the last one. If the has been a spike
    // (s->lastSpikeTime > -1) then increment s->next.  We need this,
    // since several csimSpikingInputClass objects can access the same
    // csimInputChannel and next is stored per channel.
    if ( (s->lastSpikeTime > -1) &&  (s->lastSpikeTime < SimulationTime) ) {
      s->lastSpikeTime = -1;
      (s->next)++;
    }
    // fire a spike if there is one available and SimulationTime is
    // greater or equal then the spike time
    if ( (s->next < s->length) && (s->data[s->next] <= SimulationTime) ) {
      s->lastSpikeTime = SimulationTime;
      return 1;
    }
  }
  return 0;
}

/********************************************************************************
 ** ANALOG INPUTS
 *******************************************************************************/

double csimAnalogInputClass::nextValue(int i)
{
  csimInputChannel *s;
  if ( (s=getChannel(i)) != 0 ) {
    int k=(int)(SimulationTime/s->dt + 0.5);
    if ( k<s->length )
      return s->data[k];
    else
      return s->data[s->length];
  } else
    return 0.0;
}

int csimAnalogInputClass::addInputChannel(csimInputChannel *s)
{
  if ( s->dt < 0 )
    { TheCsimError.add("AnalogInputNeuron::setInput: no analog channel!"); return -1; }

  //printf("csimAnalogInputClass::addInputChannel %i\n",s->idx[0]);

  csimInputClass::addChannel(s);

  return 0;
}
