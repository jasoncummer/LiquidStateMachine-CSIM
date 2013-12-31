#include <stdlib.h>
#include <math.h>
#include "iongate.h"
#include "ionchannel.h"
#include "csimerror.h"


double *IonGate::C1=0;
double *IonGate::C2=0;

int IonGate::addIncoming(Advancable *a)
{
  IonChannel *c=dynamic_cast<IonChannel *>(a);
  if ( c ) {
    return 0;
  } else {
    TheCsimError.add("IonGate::addIncoming: accept only IonChannels as incoming objects (not a %s)!\n",a->className());
    return -1;
  }
}

int IonGate::addOutgoing(Advancable *)
{
  return 0;
}
    

