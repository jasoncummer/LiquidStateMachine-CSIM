/*! \file dynamicstdpsynapse.h
**  \brief Class definition of DynamicStdpSynapse
*/

#ifndef _DYNAMICSTDPSYNAPSE_H_
#define _DYNAMICSTDPSYNAPSE_H_

#include "stdpsynapse.h"
#include "spikingneuron.h"
#include <math.h>

//!  Synapse with spike time dependent plasticity as well as short term dynamics (faciliataion and depression)
/** See DynamicSpikingSynapse and StdpSynapse for details */
class DynamicStdpSynapse : public StdpSynapse {

  DO_REGISTERING

public:

  #include "dynamicsynapse.h"

public:
  DynamicStdpSynapse(void);
  virtual ~DynamicStdpSynapse(void);

  virtual void reset(void);

  //! Increase psr according the Markrams model
  virtual void stdpChangePSR(void) { DYNAMIC_PSR_CHANGE;  }

  
};

#endif

