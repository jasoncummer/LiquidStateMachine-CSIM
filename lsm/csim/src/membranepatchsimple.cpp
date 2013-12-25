/*! \file membranepatchsimple.cpp
**  \brief Implementation of MembranePatchSimple
*/
#include "randgen.h"
#include "membranepatchsimple.h"
#include "synapse.h"


MembranePatchSimple::MembranePatchSimple(void) :
  Cm((float)3e-8), Rm((float)1e6), 
  Vresting ((float)-0.06),
  Vinit(Vresting),
  VmScale((float)15e-3),
  Inoise((float)0.0),
  Iinject((float)0.0)
{
  channels = 0;
  nChannels = lChannels = 0;
}

MembranePatchSimple::~MembranePatchSimple(void)
{
  if (channels) { free(channels); channels=0; }
}

//! Resets the MembranePatchSimple.
/**
 ** - \f$V_m\f$ is set to \f$V_{init}\f$ 
 ** 
 ** - \f$E_m\f$ is calculated such that for no input \f$V_m\f$ relaxes
 **   to \f$V_{resting}\f$:
 **   - \f$E_m = R_m \cdot \left( V_{resting}  G_{tot} - I_{ch} \right) \f$
 **   - \f$G_{tot} = \frac{1}{R_m} + \sum_{c=1}^{N_c} g_\infty(V_{resting})\f$
 **   - \f$I_{ch}  = \sum_{c=1}^{N_c} g_\infty(V_{resting}) E_{rev}^c \f$
 ** 
 */
void MembranePatchSimple::reset(void)
{
  // init membrane voltage first since the channels check its range!
  Vm = Vresting;

  // reset all channels and their gates
  int c;
  for(c=0;c<nChannels;c++)
    channels[c]->reset();

  // now we set Em such the Vm actually will releax to Vresting
  double Gtot = 1/Rm;
  double Ich  = 0;
  for(c=0;c<nChannels;c++) {
    Gtot += (channels[c]->gInfty());
    Ich  += (channels[c]->gInfty()*(channels[c]->Erev));
  }
  Em = Rm * (Vresting*Gtot-Ich);

  Vm = Vinit;
  
  printf("MembranePatchSimple::reset Gtot:%g\n",Gtot);
}

void MembranePatchSimple::IandGtot(double *Itot, double *Gtot)
{
  int c;

  // first we advance all our channels
  for(c=0;c<nChannels;c++)
    channels[c]->advance();

  (*Itot) = Em/Rm + Iinject;
  if ( Inoise > 0.0 )
    (*Itot) += (normrnd()*Inoise);

  //  (*Itot) += summationPoint;
  //  summationPoint = 0;

  (*Gtot)=1/Rm;
  for(c=0;c<nChannels;c++) {
    (*Gtot) += (channels[c]->g);
    (*Itot) += ((channels[c]->g)*(channels[c]->Erev));
  }
}


void MembranePatchSimple::addChannel(IonChannel *c)
{
  if ( ++nChannels > lChannels ) {
    lChannels += 2;
    channels = (IonChannel **)realloc(channels,lChannels*sizeof(IonChannel *));
  }
  channels[nChannels-1] = c;
}

void MembranePatchSimple::destroyChannels(void)
{
  for(int i=0;i<nChannels;i++)
    delete channels[i];
}

//  int MembranePatchSimple::addIncoming(Advancable *a)
//  {
//    IonChannel *c=dynamic_cast<IonChannel *>(a);
//    if ( c ) {
//      addChannel(c); return 0;
//    } else {
//      return Neuron::addIncoming(a);
//    }
//  }

//  int MembranePatchSimple::addOutgoing(Advancable *a)
//  {
//    IonChannel *c=dynamic_cast<IonChannel *>(a);
//    if ( c ) {
//      return 0;
//    } else {
//      return Neuron::addIncoming(a);
//    }
//  }

