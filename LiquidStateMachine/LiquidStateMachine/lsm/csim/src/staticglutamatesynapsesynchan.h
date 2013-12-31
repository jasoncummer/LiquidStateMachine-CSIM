/*! \file staticglutamatesynapsesynchan.h
**  \brief Class definition of StaticGlutamateSynapseSynchan
*/

#ifndef _STATICGLUTAMATESYNAPSESYNCHAN_H_
#define _STATICGLUTAMATESYNAPSESYNCHAN_H_

#include "glutamatesynapsesynchan.h"

//! A STDP synapse with no synaptic short time dynamics, i.e. no depression and no facilitation
class StaticGlutamateSynapseSynchan : public GlutamateSynapseSynchan  {

  DO_REGISTERING

public:
  StaticGlutamateSynapseSynchan() {};
  
protected:
  //! Increase the psr by \f$W\f$.
  virtual void stdpChangePSR(void) { }
  //virtual void stdpChangePSR(void) { psr += W; }

};

#endif
