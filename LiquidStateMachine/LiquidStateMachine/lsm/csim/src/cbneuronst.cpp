/*! \file cbneuronst.cpp
**  \brief Implementation of CbNeuron
*/

#include <string.h>
#include <stdio.h>
#include "randgen.h"
#include "cbneuronst.h"

//! Normalized spike template defining the membrane voltage \f$Vm\f$ during the refractory period \internal [hidden]
const double CbNeuronSt::STEMP[]  = {0.12081, 0.20557, 0.20557, 0.16519, 0.16519, 0.093609, 0.022026, 0.022026,
				-0.057621, -0.057621, -0.070240, -0.082859, -0.082859, -0.073702,
				-0.073702, -0.064433, -0.055165, -0.055165, -0.039308, -0.039308,
				-0.033389, -0.027471, -0.027471, -0.018984, -0.018984, -0.016192,
				-0.013400, -0.013400, -0.009157, -0.009157, -0.008040, -0.006924,
				-0.006924, -0.004914, -0.004914, -0.004132, -0.003350, -0.003350,
				-0.002010, -0.002010, -0.001787, -0.001563, -0.001563, -0.001563,
				-0.001563, -0.001340, -0.001117, -0.001117, -0.000223, -0.000223};

 //const double CbNeuronSt::NSTEMP = 50;
const int CbNeuronSt::NSTEMP = sizeof(CbNeuronSt::STEMP)/sizeof(CbNeuronSt::STEMP[0]);



#define SPIKE_OCCURED  /* this has to be done if a spike occured */ \
		       nStepsInRefr = (int)(Trefract/DT+0.5); \
		       STempIdxMax = nStepsInRefr-1; \
		       STempIdx = 0; \
                       for(int c=0;c<nChannels;c++) { \
		         channels[c]->membraneSpikeNotify(SimulationTime); \
		       }

double CbNeuronSt::nextstate(void)
{
  Isyn = summationPoint;
  Gsyn = GSummationPoint;

 // first we advance all our channels
  double Itot, Gtot;
  MembranePatch::IandGtot(&Itot,&Gtot);

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
    /* Follow spike template as long as we are refractory
      (if STempIdx = NSTEMP then Vm stays constant) */
//    if ((doReset > 0.0)&(STempIdx < NSTEMP)) {
    if ((doReset > 0.0)&&(STempIdx <= STempIdxMax)) {
       int i,imax;
       imax = (int)((STempIdx*(NSTEMP-1))/STempIdxMax);

       if (nStepsInRefr==0) {
          Vm = Vreset;
       } else {
          Vm = Vthresh;
          for(i=0;i<=imax;i++) {
             Vm += STEMP[i] > 0 ? STEMP[i]*STempHeight : STEMP[i]*(STempHeight + Vthresh - Vreset);
          }
       }
       STempIdx++;
    }

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
