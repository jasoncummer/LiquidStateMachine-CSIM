/*! \file lifneuronsynchan.h
**  \brief Class definition of LifNeuronSynchan
*/

#ifndef _LIFNEURONSYNCHAN_H_
#define _LIFNEURONSYNCHAN_H_

#include "csimclass.h"
#include "lifneuron.h"

//! A leaky-integrate-and-fire (I&F) neuron.
/** 
 ** \latexonly  \subsubsection*{Model} \endlatexonly
 ** \htmlonly  <h3>Model</h3> \endhtmlonly
 ** 
 ** A standard leaky-integrate-and-fire neuron model is implemented
 ** where the membrane potential \f$V_m\f$ of a neuron is given by 
 ** \f[
 **   \tau_m \frac{d V_m}{dt} = -(V_m-V_{resting}) + R_m \cdot (I_{syn}(t)+I_{inject}+I_{noise})
 ** \f] 
 ** where \f$\tau_m=C_m\cdot R_m\f$ is the membrane time constant,
 ** \f$R_m\f$ is the membrane resistance, \f$I_{inject}\f$ is a
 ** non-specific background current and \f$I_{noise}\f$ is a
 ** Gaussion random variable with zero mean and a given variance
 ** noise. 
 **
 ** This neuron efficiently implements 4 types of synaptic channels
 ** (NMDA, AMPA, GABA A, GABA B).  plus the standard synaptic cannel.
 ** The synaptic current \f$I_syn\f$ is the sum of the 5 currents from
 ** the syn. channels.  For NMDA, AMPA, GABA A, and GABA B, the
 ** synaptic conductance is increased by each synapse (see synapses)
 ** and then decays with the respective time constants
 ** \f$tau_{channel}\f$. The current is given by \f$g_{channel} \cdot
 ** (E_{channel}-V_m)\f$.
 **
 ** At time \f$t=0\f$ \f$V_m\f$ ist set to \f$V_{init}\f$. If
 ** \f$V_m\f$ exceeds the threshold voltage \f$V_{thresh}\f$ it is
 ** reset to \f$V_{reset}\f$ and hold there for the length
 ** \f$T_{refract}\f$ of the absolute refractory period.
 **
 ** \latexonly  \subsubsection*{Implementation} \endlatexonly
 ** \htmlonly  <h3>Implementation</h3> \endhtmlonly
 **
 ** The exponential Euler method is used for numerical integration.
 ** 
 ** */
class LifNeuronSynchan : public LifNeuron {

 DO_REGISTERING  

 public:

  LifNeuronSynchan(void);
  virtual ~LifNeuronSynchan();

  //! Recalculate exp. Euler constants
  virtual int updateInternal(void);

  //! Reset the leaky I&F neuron.
  virtual void reset(void);

  //! Calculate the new membrane voltage and check wheter Vm exceeds the threshold V_thresh.
  virtual double nextstate(void);

  //! Returns the actual membrane voltage
  virtual float getVm(void) {return Vm;};

  //! The time constant for NMDA channels. [units=sec; range=(0,1000);]
  float tau_nmda;

  //! The time constant for AMPA channels. [units=sec; range=(0,1000);]
  float tau_ampa;

  //! The time constant for GABA_A channels. [units=sec; range=(0,1000);]
  float tau_gaba_a;

  //! The time constant for GABA_B channels. [units=sec; range=(0,1000);]
  float tau_gaba_b;

  //! The reversal potential for NMDA channels. [units=V;]
  float E_nmda;

  //! The reversal potential for AMPA channels. [units=V;]
  float E_ampa;

  //! The reversal potential for GABA_A channels. [units=V;]
  float E_gaba_a;

  //! The reversal potential for GABA_B channels. [units=V;]
  float E_gaba_b;

  //! Mg-concentration for voltage-dependence of NMDA-channel in [units=mMol]
  float Mg_conc;

protected:
  friend class GlutamateSynapseSynchan;
  friend class DynamicGlutamateSynapseSynchan;

  //! \internal factor for exponential decay of the nmda state variable \f$x(t)\f$. [hidden;]
  /** \internal It is initialized to \f$\exp(-dt/\tau_{nmda})\f$ where \f$dt\f$ is the
      integration time constant. */
  double decay_nmda;
  //! \internal factor for exponential decay of the ampa state variable \f$x(t)\f$. [hidden;]
  double decay_ampa;
  //! \internal factor for exponential decay of the gaba a state variable \f$x(t)\f$. [hidden;]
  double decay_gaba_a;
  //! \internal factor for exponential decay of the gaba b state variable \f$x(t)\f$. [hidden;]
  double decay_gaba_b;

  //! At this point synaptic input from NMDA-channels is summed up. [readonly]
  double summationPoint_nmda;
  //! At this point synaptic input from AMPA-channels is summed up. [readonly]
  double summationPoint_ampa;
  //! At this point synaptic input from GABA_A-channels is summed up. [readonly]
  double summationPoint_gaba_a;
  //! At this point synaptic input from GABA_B-channels is summed up. [readonly]
  double summationPoint_gaba_b;
};

#endif
