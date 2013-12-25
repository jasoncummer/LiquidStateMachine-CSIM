#include <stdlib.h>
#include "activechannel.h"
#include "iongate.h"
#include "cbneuron.h"
#include "csimerror.h"
#include "analoginputneuron.h"
#include "membranepatchsimple.h"

#include "hh_squid_channels.h"

ActiveChannel::ActiveChannel(void)
{
  lGates = nGates = 0;
  gates = 0;
  Gbar = 0;
}

ActiveChannel::~ActiveChannel()
{
  if (gates) free(gates); gates = 0;
}

void ActiveChannel::reset(void)
{
  g = Gbar;
  for(int i=0;i<nGates;i++) {
    gates[i]->reset();
    g *= (gates[i]->P);
  }
}

int ActiveChannel::updateInternal(void)
{
  g = Gbar;
  for(int i=0;i<nGates;i++) {
    if ( gates[i]->updateInternal() < -1 ) return -1;
    g *= (gates[i]->P);
  }
  return 0;
}

int ActiveChannel::advance(void)
{
  g = Gbar;
  for(int i=0;i<nGates;i++) {
    gates[i]->advance();
    g *= (gates[i]->P);
  }
  return 1;
}

double ActiveChannel::gInfty(void)
{
  if ( membrane==0 ){
    TheCsimError.add("ActiveChannel::gInfty: Channel not connected to any membrane!\n");
    return 0;
  }

  double ginfty = Gbar;
  for(int i=0;i<nGates;i++) {
    ginfty *= (gates[i]->pInfty(membrane));
  }
  return ginfty;
}

void ActiveChannel::addGate(IonGate *c)
{
  if ( ++nGates > lGates ) {
    lGates += 2;
    gates = (IonGate **)realloc(gates,lGates*sizeof(IonGate *));
  }
  gates[nGates-1] = c;
}

int ActiveChannel::addIncoming(Advancable *Incoming)
{

/* DOESNT WORK WITH MEMBRANEPATCH CONCEPT
  AnalogInputNeuron *ai=dynamic_cast<AnalogInputNeuron *>(Incoming);
  if ( ai ) {
    Vm=&(ai->Vm); Vresting=&(ai->Vresting);
    for(int i=0;i<nGates;i++)
      gates[i]->setV(Vm,Vresting);
    return 0;
  }
*/

  IonGate *g=dynamic_cast<IonGate *>(Incoming);
  if ( g ) {
    addGate(g);
    gates[nGates-1]->ConnectToMembrane(membrane);
    return 0;
  }


  MembranePatchSimple *m=dynamic_cast<MembranePatchSimple *>(Incoming);
  if ( m ) {
    membrane = m;

    for(int i=0;i<nGates;i++)
      gates[i]->ConnectToMembrane(membrane);
    return 0;
  } else {
    TheCsimError.add("ActiveChannel::addIncoming: accept only Iongate and MembranePatchSimple (not %s)!",
		     Incoming->className());
    return -1;
  }
}

int ActiveChannel::addOutgoing(Advancable *)
{
  return 0;
}
