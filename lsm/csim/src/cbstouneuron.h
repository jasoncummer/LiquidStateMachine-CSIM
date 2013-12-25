/*! \file cbstouneuron.h
**  \brief Implementation of conductance based neurons with Ornstein Uhlenbeck process noise
*/

#ifndef _CB_ST_OU_NEURON_H_
#define _CB_ST_OU_NEURON_H_

#include "cbneuronst.h"
#include "spikingneuron.h"
#include "ionchannel.h"

//! A single compartment neuron with an arbitrary number of channels, coductance based as well as current based synapses, a spike template and \b Ornstein \b Uhlenbeck \b process \b noise
/** 
 ** \latexonly \subsubsection*{The Model} \endlatexonly
 ** \htmlonly <h3>Model</h3> \endhtmlonly
 ** 
 ** The membrane voltage \f$V_m\f$ is governed by 
 ** \f[
 ** C_m \frac{V_m}{dt} = -\frac{V_m-E_m}{R_m} - \sum_{c=1}^{N_c} g_c(t) ( V_m - E_{rev}^c ) + \sum_{s=1}^{N_s} I_s(t) + \sum_{s=1}^{G_s} g_s(t)(V_m-E_{rev}^{(s)}) + g_{noise}^{exz}(t)(V_m -E_{noise}^{exc}) + g_{noise}^{inh}(t)(V_m -E_{noise}^{inh}) + I_{inject}
 ** \f]
 ** with the following meanings of symbols
 ** - \f$C_m\f$ membrane capacity (Farad)
 ** - \f$E_m\f$ reversal potential of the leak current (Volts)
 ** - \f$R_m\f$ membrane resistance (Ohm)
 ** - \f$N_c\f$ total number of channels (active + synaptic)
 ** - \f$g_c(t)\f$ current conductance of channel \f$c\f$ (Siemens)
 ** - \f$E_{rev}^c\f$ reversal potential of channel \f$c\f$ (Volts)
 ** - \f$N_s\f$ total number of current supplying synapses
 ** - \f$I_s(t)\f$  current supplied by synapse \f$s\f$ (Ampere)
 ** - \f$G_s\f$ total number of coductance based synapses
 ** - \f$g_s(t)\f$ coductance supplied by synapse \f$s\f$ (Siemens)
 ** - \f$E_{rev}^{(s)}\f$ reversal potential of synapse \f$s\f$ (Volts)
 ** - \f$g_{noise}^{exz}(t)\f$ excitatory coductance given Ornstein Uhlenbeck process noise
 ** - \f$E_{noise}^{exc}\f$ reversal potential for excitatory Ornstein Uhlenbeck process noise
 ** - \f$g_{noise}^{inh}(t)\f$ inhibitory coductance given Ornstein Uhlenbeck process noise
 ** - \f$E_{noise}^{inh}\f$ reversal potential for inhibitory Ornstein Uhlenbeck process noise
 ** - \f$I_{inject}\f$ injected current (Ampere)
 **
 ** At time \f$t=0\f$ \f$V_m\f$ ist set to \f$V_{init}\f$.
 **
 ** The value of \f$E_m\f$ is calculated to compensate for ionic
 ** currents such that \f$V_m\f$ actually has a resting value of
 ** \f$V_\mathit{resting}\f$.
 **
 ** \latexonly  \subsubsection*{Spiking and reseting the membrane voltage} \endlatexonly
 ** \htmlonly  <h3>Spiking and reseting the membrane voltage</h3> \endhtmlonly
 **
 ** If the membrane voltage \f$V_m\f$ exceeds the threshold
 ** \f$V_{tresh}\f$ the CbNeuronSt sends a spike to all its outgoing
 ** synapses and the \b membrane \b voltage \b follows a \b predefined \b spike \b templage 
 ** during the absolute
 ** refractory period of length \f$T_{refract}\f$ \b if \b doReset \b = \b 1.
 **
 ** If the flag \c doReset=0 the spike template is not applied and
 ** the above equation is also applied during the absolute refractory
 ** period but the event of threshold crossing is transmitted as a
 ** spike to outgoing synapses. This is usfull if one includes
 ** channels which produce a \em real action potential (see
 ** HH_K_Channel and HH_Na_Channel) but one still just wants to
 ** communicate the spikes as events in time.
 **
 ** \latexonly  \subsubsection*{Implementation} \endlatexonly
 ** \htmlonly  <h3>Implementation</h3> \endhtmlonly
 **
 ** The exponential Euler method is used for numerical integration.
 **
 ***/
class CbStOuNeuron : public CbNeuronSt {

 DO_REGISTERING

 public:

  CbStOuNeuron(void);
  virtual ~CbStOuNeuron() {};

  virtual double nextstate(void);

  virtual void reset(void);

  //! exc and inh conductances (noise) [readwrite; units=S;]
  double ge,gi;

  //! exc and inh mean conductances (noise) [readwrite; units=S;]
  double ge0,gi0;

  //! time constants and std for exc and inh conductances (noise) [readwrite; units=S;]
  double tau_e,tau_i,sig_e,sig_i;

  //! Reversal potential for exc and inh currents (noise) [readwrite; units=V;]
  double Ee,Ei;

 protected:

 //! noise input current
  double OuInoise;

 //! noise input conductance
  double OuGnoise;

 private:

  //! constant for Ornstein Uhlenbeck process [hidden;]
  double Ae,Ai,Ce,Ci,De,Di;

};

#endif
