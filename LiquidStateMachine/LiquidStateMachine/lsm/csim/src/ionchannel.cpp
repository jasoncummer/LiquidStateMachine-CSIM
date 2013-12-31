/*! \file ionchannel.cpp
**  \brief Implementation of IonChannel
*/

#include "ionchannel.h"
#include "cbneuron.h"
#include "analoginputneuron.h"
#include "membranepatch.h"
#include "csimerror.h"

int IonChannel::addIncoming(Advancable *Incoming)
{
/* DOESNT WORK WITH MEMBRANEPATCH CONCEPT
  // this can be used as a source of Vm and Vresting to implement
  // e.g. teacher forcing when fitting models to data

  AnalogInputNeuron *ai=dynamic_cast<AnalogInputNeuron *>(Incoming);
  if ( ai ) {
    Vm=&(ai->Vm); Vresting=&(ai->Vresting); return 0;
  }
*/

  MembranePatch *m=dynamic_cast<MembranePatch *>(Incoming);
  if ( m ) {
    membrane = m; return 0;
  } else {
    TheCsimError.add("IonChannel::addIncoming: accept only MembranePatch (not %s)!",
		     Incoming->className());
    return -1;
  }
}

int IonChannel::addOutgoing(Advancable *)
{
  return 0;
}
