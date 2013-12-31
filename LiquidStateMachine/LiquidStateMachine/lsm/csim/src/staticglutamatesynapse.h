/*! \file staticglutamatesynapse.h
**  \brief Class definition of StaticGlutamateSynapse
*/

#ifndef _STATICGLUTAMATESYNAPSE_H_
#define _STATICGLUTAMATESYNAPSE_H_

#include "glutamatesynapse.h"

//! A Glutamate synapse with no synaptic short time dynamics, i.e. no depression and no facilitation
class StaticGlutamateSynapse : public GlutamateSynapse  {

  DO_REGISTERING

public:
  StaticGlutamateSynapse() {};
  
protected:
  //! Increase the psr by \f$W\f$.
  virtual void stdpChangePSR(void) { 
    psr += (fact_ampa*W/decay); 
    psr_nmda += (fact_nmda*W/decay_nmda);
  }
};

#endif
