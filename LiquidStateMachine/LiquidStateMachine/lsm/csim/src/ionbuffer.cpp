/*! \file ionbuffer.cpp
**  \brief Implementation of an ion buffer
*/

#include <math.h>
#include "ionbuffer.h"


IonBuffer::IonBuffer(float ConcRest_def, float ConcOut_def, float Tau_def, float Temp_def, int zIon_def) :
  ConcScale((float)1.0),
  Erev((double)0.0),
  active((bool)0),
  ConcActive((bool)0),
  ErevActive((bool)0),
  C1((double)0.0)
{
  ConcRest = ConcRest_def;
  ConcOut = ConcOut_def;
  Tau = Tau_def;
  Temp = Temp_def;
  zIon = zIon_def;

  channels = 0;
  nChannels = lChannels = 0;

  Conc = ConcRest;
}

IonBuffer::~IonBuffer(void)
{
  if (channels) { free(channels); channels=0; }
}

void IonBuffer::SetErev(double Vresting, double VmScale)
{
  // Option 1: complete Nernst equation
  Erev = R*Temp/(zIon*FARADAY)*log(ConcOut/Conc); // [V]

  // Option 2: Nernst equation for const temperatur of 37 degree celcius
  // Erev = 12.5e-3*log(ConcOut/Conc); // [V]

  // rescale to voltage range of the model
  Erev = (Erev - V_REST_BIOL)*VmScale/(V_THRESH_BIOL-V_REST_BIOL) + Vresting;
}


//! Resets the IonBuffer.
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
void IonBuffer::reset(double *Gtot, double *Ich, double Vresting, double VmScale)
{
  Conc = ConcRest;
  C1 = exp(-DT/Tau);

  if (ErevActive)
       SetErev(Vresting,VmScale);


  // reset all channels and their gates
  int c;
  for(c=0;c<nChannels;c++) {
    channels[c]->reset();
    // dont use ginfty() instead of g because some gates could depend on yet
    // not reseted Conc variables of other IonBuffers!
    // (g is set correctly by reset(), where gate->reset() use ConcRest and infty(*Vm))
    (*Gtot) += (channels[c]->g);
    (*Ich)  += (channels[c]->g*(channels[c]->Erev));
  }

  // For a faster check if nextstate is needed
  // Note: not identical with ConcActive, because channels can be present
  //       and have to be advanced in nextstate, but Conc isn't needed.
  active = (nChannels>0);
}

#define A_MEMBRANE 0.01e-2 // [cm2]

void IonBuffer::nextstate(double *Imtot, double *Gmtot, double Vm, double Vresting, double VmScale)
{
  int c;

  // first we advance all our channels
  for(c=0;c<nChannels;c++)
    channels[c]->advance();

  double Itot = 0;
  double Gtot = 0;

  for(c=0;c<nChannels;c++) {
    Gtot += (channels[c]->g);
    Itot += ((channels[c]->g)*(channels[c]->Erev));
  }

  // add specific ion current to the total membrane current
  (*Gmtot) += Gtot;
  (*Imtot) += Itot;

  if (ConcActive) {
    // do the exponential Euler integration step for the ion concentration
    double drive = 1e-2*1e3*(Itot - Gtot*Vm)/(2*FARADAY*DEPTH*A_MEMBRANE); // 1e-2 because DEPTH is in unit [m]; 1e7 to convert [A] -> [mA]
    Conc = C1*Conc+(1-C1)*(Tau*drive + ConcRest);

    if (ErevActive)
       SetErev(Vresting,VmScale);
  }
}




void IonBuffer::addChannel(IonChannel *c)
{
  if ( ++nChannels > lChannels ) {
    lChannels += 2;
    channels = (IonChannel **)realloc(channels,lChannels*sizeof(IonChannel *));
  }
  channels[nChannels-1] = c;
}

