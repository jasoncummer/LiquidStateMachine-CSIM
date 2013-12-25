/*! \file dynamicglutamatesynapsesynchan.h
**  \brief Class definition of DynamicGlutamateSynapseSynchan
*/

#ifndef _DYNAMICGLUTAMATESYNAPSESYNCHAN_H_
#define _DYNAMICGLUTAMATESYNAPSESYNCHAN_H_

#include "glutamatesynapsesynchan.h"
#include "spikingneuron.h"
#include <math.h>

//! Base class for all dynamic spiking synapses with spike time dependent plasticity (STDP)
class DynamicGlutamateSynapseSynchan : public GlutamateSynapseSynchan {

  DO_REGISTERING

public:

  #include "dynamicsynapse.h"

public:
  DynamicGlutamateSynapseSynchan(void);
  virtual ~DynamicGlutamateSynapseSynchan(void);

  virtual void reset(void);

  //! Increase psr according the Markrams model
  virtual void stdpChangePSR(void) { }
  
  virtual int preSpikeHit(void);
  
};

#endif
