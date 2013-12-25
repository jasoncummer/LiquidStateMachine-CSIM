/*! \file glutamatesynapse.h
**  \brief Class definition of GlutamateSynapse with NMDA and AMPA-Receptors.
  */

#ifndef _GLUTAMATESYNAPSE_H_
#define _GLUTAMATESYNAPSE_H_

#include "stdpsynapse.h"

//! Base Class for Glutamate synapse with NMDA and AMPA channels. 
/** This is a slow version of a glutamate-synapse.
 ** NMDA and AMPA channels are conductance based 
 ** Attention: Neuron needs biological resting potential (ca. -70mV). 
 ** Note that this synapse can not be used as an inhibitory synapse! 
 ** Since channels are conductance based, weight has to be much larger (about 50x) 
  */
class GlutamateSynapse : public StdpSynapse {

  DO_REGISTERING

public:
  GlutamateSynapse();
  virtual int updateInternal(void);
  virtual int advance(void);
  virtual int preSpikeHit(void);
  virtual int addOutgoing(Advancable *a);
  virtual void reset(void);

  //!The NMDA time constant \f$\tau_{nmda}\f$ [units=sec; range=(0,100)];
  /** A spike causes a exponential decaying postsynaptic response of
   ** the form \f$\exp(-t/\tau_{nmda})\f$ */
  float tau_nmda;

  //! Mg-concentration for voltage-dependence of NMDA-channel in [units=mMol]
  float Mg_conc;

  //! Reversal Potential for NMDA-Receptors [units=V]
  float E_nmda;

  //! Reversal Potential for AMPA-Receptors [units=V]
  float E_ampa;

  //!The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.
  float fact_nmda;
  //!The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.
  float fact_ampa;

  //! The psr (postsynaptic response) for nmda channels. \internal [units=; readonly;]
  double psr_nmda;

  inline bool checkForActivation(void);
  
protected:
  //! \internal factor for exponential decay of the nmda state variable \f$x(t)\f$. [hidden;]
  /** \internal It is initialized to \f$\exp(-dt/\tau_{nmda})\f$ where \f$dt\f$ is the
      integration time constant. */
  double decay_nmda;

};

#endif
