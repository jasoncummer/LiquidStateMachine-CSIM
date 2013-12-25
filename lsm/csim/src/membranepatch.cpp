/*! \file membranepatch.cpp
**  \brief Implementation of MembranePatch
*/

#include "randgen.h"
#include "membranepatch.h"
#include "synapse.h"
#include "activecachannel.h"

MembranePatch::MembranePatch(void) {
  buffers = 0;
  nBuffers = lBuffers = 0;

  addBuffer(new IonBuffer(2e-7,2e-3,200e-3,309.15,2)); // Ca++ buffer
}

MembranePatch::~MembranePatch(void)
{
  if (buffers) { free(buffers); buffers=0; }
}

void MembranePatch::reset(void)
{
  // init membrane voltage first since the channels check its range!
  Vm = Vresting;

  double Gtot = 1/Rm;
  double Ich  = 0;

  // reset g=ginfty of all unspecific channels and their Vm or Conc dep. gates.
  // Vm is already Vinit and ConcIonGate::reset() uses only ConcReset and not the 
  // yet not reseted Conc variables
  int c;
  for(c=0;c<nChannels;c++) {
    channels[c]->reset();

    // dont use ginfty() instead of g because some gates could depend on yet
    // not reseted Conc variables of other IonBuffers!
    // (g is set correctly by reset(), where gate->reset() use ConcRest and infty(*Vm))
    Gtot += (channels[c]->g);
    Ich  += (channels[c]->g*(channels[c]->Erev));

    // printf("MembranePatch::reset %g\n",(channels[c]->g*(channels[c]->Erev)));
  }

  // printf("MembranePatch::reset Gtot:%g\n",Gtot);

  // reset all buffers: Conc, Erev and the specific ion channels
  int b;
  for(b=0;b<nBuffers;b++)
    buffers[b]->reset(&Gtot,&Ich,Vresting,VmScale);

  // now we set Em such the Vm actually will releax to Vresting
  Em = Rm * (Vresting*Gtot-Ich);

  Vm = Vinit;
}

void MembranePatch::IandGtot(double *i, double *g)
{
  // do first simple membrane stuff
  MembranePatchSimple::IandGtot(i,g);

  // then the stuff of the specific ion channel types
  // (Note: This is the only additional overhead if no IonBuffer is present)
  int b;
  for(b=0;b<nBuffers;b++) {
    if (buffers[b]->active)
       buffers[b]->nextstate(i,g,Vm,Vresting,VmScale);
  }
}


void MembranePatch::addBuffer(IonBuffer *b)
{
  if ( ++nBuffers > lBuffers ) {
    lBuffers += 2;
    buffers = (IonBuffer **)realloc(buffers,lBuffers*sizeof(IonBuffer *));
  }
  buffers[nBuffers-1] = b;
}

void MembranePatch::addChannel(IonChannel *c)
{
  // check if channel should be managed by a specific ion buffer or the membrane
  ActiveCaChannel *cac = dynamic_cast<ActiveCaChannel *>(c);
  if (cac != NULL) {
    buffers[CA]->addChannel(c);
    return;
  }


  if ( ++nChannels > lChannels ) {
    lChannels += 2;
    channels = (IonChannel **)realloc(channels,lChannels*sizeof(IonChannel *));
  }
  channels[nChannels-1] = c;
}

