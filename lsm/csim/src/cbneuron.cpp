/*! \file cbneuron.cpp
**  \brief Implementation of CbNeuron
*/

#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "cbneuron.h"
#include "synapse.h"

CbNeuron::CbNeuron(void) :
  Vreset(0),
  Trefract((float)3e-3)
{
  Vthresh = (float)(Vresting+0.015);
  channels = 0;
  nChannels = lChannels = 0;
  doReset = 1;
  GSummationPoint = 0;    
  nummethod = 0;
}

CbNeuron::~CbNeuron(void)
{
  if (channels) { free(channels); channels = 0; }
}

//! Resets the CbNeuron.
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
void CbNeuron::reset(void)
{
  SpikingNeuron::reset(); // init the base spiking stuff first
  spike         = 0;      // no spikes yet
  nStepsInRefr  = -1;     // we are not refractory at the begining
  GSummationPoint = 0;    // clear synatpic conductances

  MembranePatch::reset();

  // init const C1 for solution of the differential equation

  if (nummethod) {
    /* Crank-Nicolson integration step */
    C1 = DT/(2*Cm);    

    /* Advance Vm to staggered grid at DT/2 */
    double Itot, Gtot;
    IandGtot(&Itot,&Gtot); // advances all ion channels parameters one full step
    Vm = (Vm + C1*Itot)/(1 + C1*Gtot);  // advances Vm one backward Euler half step
// printf("CbNeuron::reset Crank-Nicolson \n");
  } else {
    /* exponential Euler integration step */
    C1 = ( Cm*Rm > 0 ) ? exp(-DT/(Cm*Rm)) : 0.0 ;
// printf("CbNeuron::reset exponential Euler\n");
  }
}

#define SPIKE_OCCURED  /* this has to be done if a spike occured */ \
                       nStepsInRefr = (int)(Trefract/DT+0.5); \
		       Vm = doReset ? Vreset : Vm ; \
                       for(int c=0;c<nChannels;c++) { \
		         channels[c]->membraneSpikeNotify(SimulationTime); \
		       }

double CbNeuron::nextstate(void)
{
  Isyn = summationPoint;
  Gsyn = GSummationPoint;

  // first we advance all our channels
  double Itot, Gtot;
  IandGtot(&Itot,&Gtot);

  if ( (!doReset) || (nStepsInRefr<0) ) {

    Itot += summationPoint;
    Gtot += GSummationPoint;

    /* do the integration step of the differential equation */

    if (nummethod) {
       /* do the Crank-Nicolson integration step */
      
       double V12;
       V12 = (Vm + C1*Itot)/(1 + C1*Gtot); // backward Euler half step
       Vm  = 2*V12 - Vm;                   // forward Euler half step

    } else {
       /* do the exponential Euler integration step */
       C1 = exp(-DT/(Cm/Gtot));
       Vm = C1*Vm+(1-C1)*(Itot/Gtot);
    }
  }
  hasFired = 0;

  if ( (nStepsInRefr)-- > 0) {
    /* stay at 'Vreset' as long as we are refractory */
    Vm = doReset ? Vreset : Vm ;
  } else if ( Vm >= Vthresh ) {
      /* Note that we want to spike! */
      hasFired = 1;
      // do what is necessary to internally process the spike
      SPIKE_OCCURED;
  }

  SpikingNeuron::nextstate();

  summationPoint = 0;
  GSummationPoint = 0;
  return Vm;
}

int CbNeuron::isRefractory(void)
{
  return (nStepsInRefr > 0);
}

int CbNeuron::addIncoming(Advancable *a)
{
  IonChannel *c=dynamic_cast<IonChannel *>(a);
  if ( c ) {
    addChannel(c); return 0;
  } else {
    return SpikingNeuron::addIncoming(a);
  }
}

int CbNeuron::addOutgoing(Advancable *a)
{
  IonChannel *c=dynamic_cast<IonChannel *>(a);
  if ( c ) {
    return 0;
  } else {
    return SpikingNeuron::addOutgoing(a);
  }
}
