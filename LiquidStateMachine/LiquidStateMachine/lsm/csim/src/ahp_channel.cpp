#include <stdlib.h>
#include <math.h>
#include "ahp_channel.h"
#include "cbneuron.h"
#include "analogneuron.h"
#include "membranepatchsimple.h"
#include "csimerror.h"

AHP_Channel::AHP_Channel(void)
{
   Gbar=0; n=0; u=0; Ts=1;
   C1 = 0;
};


int AHP_Channel::updateInternal(void)
{
  C1 = exp(-DT/Ts);
  return 0;
}


int AHP_Channel::advance(void)
{
  g = Gbar*n;
  n = n*C1;
  return 1;
}

void AHP_Channel::membraneSpikeNotify(double )
{
  n = n + u*(1 - n);
}

void AHP_Channel::reset(void)
{
  if ( membrane!=0 ) {
    // set n to the 'resting value' at time t=0
    n = 0;
    g = 0;
  } else {
    TheCsimError.add("AHP_Channel::reset: Channel not connected to any MembranePatch!\n");
    return;
  }
}

int AHP_Channel::addIncoming(Advancable *Incoming)
{
/*  AnalogNeuron *an=dynamic_cast<AnalogNeuron *>(Incoming);
  if ( an ) {
    Vm=&(an->Vm); Vresting=&(an->Vresting);
    return 0;
  }
*/

  MembranePatchSimple *m=dynamic_cast<MembranePatchSimple *>(Incoming);
  if ( m ) {
    membrane=m;
    return 0;
  } else {
    TheCsimError.add("AHP_Channel::addIncoming: accept only MembranePatchSimple (not %s)!",
		     Incoming->className());
    return -1;
  }
}

int AHP_Channel::addOutgoing(Advancable *)
{
  return 0;
}
