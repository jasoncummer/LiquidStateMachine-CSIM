/*! \file glutamatesynapsesynchan.h
**  \brief Class definition of GlutamateSynapseSynchan with NMDA and AMPA-Receptors at the target neuron.
 */

#ifndef _GLUTAMATESYNAPSESYNCHAN_H_
#define _GLUTAMATESYNAPSESYNCHAN_H_

#include "stdpsynapse.h"

//! Base Class for Glutamate synapse with NMDA and AMPA channels at the target neuron. 
/** Needs LifNeuronSynchan as post-neuron
 ** This synapse just hands over the weighted spike to the neuron. The neuron does the rest.
 ** Since channels are conductance based, weight has to be much larger (about 50x) 
  */
class GlutamateSynapseSynchan : public StdpSynapse {

  DO_REGISTERING

public:
  GlutamateSynapseSynchan();
  virtual int advance(void);
  virtual int preSpikeHit(void);
  virtual int addOutgoing(Advancable *a);

  //!The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.
  float fact_nmda;
  //!The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.
  float fact_ampa;
  
 protected:
  
  //! Pointer to postsynaptic nmda summation point within the LifNeuronSynchannel \internal [hidden;]
  double *summationPoint_nmda;
  //! Pointer to postsynaptic ampa summation point within the LifNeuronSynchannel \internal [hidden;]
  double *summationPoint_ampa;
};

#endif
