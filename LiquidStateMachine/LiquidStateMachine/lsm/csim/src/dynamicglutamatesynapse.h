/*! \file dynamicglutamatesynapse.h
**  \brief Class definition of DynamicGlutamateSynapse
*/

#ifndef _DYNAMICGLUTAMATESYNAPSE_H_
#define _DYNAMICGLUTAMATESYNAPSE_H_

#include "glutamatesynapse.h"
#include "spikingneuron.h"
#include <math.h>

//! Base class for dynamic spiking glutamate synapses with spike time dependent plasticity (STDP)
class DynamicGlutamateSynapse : public GlutamateSynapse {

  DO_REGISTERING

public:

  #include "dynamicsynapse.h"

public:
  DynamicGlutamateSynapse(void);
  virtual ~DynamicGlutamateSynapse(void);

  virtual void reset(void);

  //! Increase psr according the Markrams model
  virtual void stdpChangePSR(void) {
    if ( lastSpike > 0 ) { 
      double isi = SimulationTime - lastSpike;
      r = 1 + (r*(1-u)-1)*exp(-isi/D);
      u = U + u*(1-U)*exp(-isi/F);
    }
    psr += ((fact_ampa*W/decay) * u * r);
    psr_nmda += ((fact_nmda*W/decay_nmda) * u * r);
  }
  
};

#endif
