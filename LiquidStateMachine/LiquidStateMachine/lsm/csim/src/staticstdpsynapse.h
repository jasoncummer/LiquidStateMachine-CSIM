/*! \file staticstdpsynapse.h
**  \brief Class definition of StaticStdpSynapse
*/

#ifndef _STATICSTDPSYNAPSE_H_
#define _STATICSTDPSYNAPSE_H_

#include "stdpsynapse.h"

//! A STDP synapse with no synaptic short time dynamics, i.e. no depression and no facilitation
class StaticStdpSynapse : public StdpSynapse  {

  DO_REGISTERING

public:
  StaticStdpSynapse() {};
  
protected:
  //! Increase the psr by \f$W\f$.
  virtual void stdpChangePSR(void) { psr += (W/decay); }

};

#endif
