/*! \file membranepatchsimple.h
**  \brief Class definition of MembranePatchSimple
*/

#ifndef _MEMBRANEPATCH_SIMPLE_H_
#define _MEMBRANEPATCH_SIMPLE_H_

#include "spikingneuron.h"
#include "ionchannel.h"
#include "synapsetarget.h"

class IonChannel;

//! A a path of membrane with an arbitrary number of channels and current supplying synapses
/**
 ** \latexonly \subsubsection*{The Model} \endlatexonly
 ** \htmlonly <h3>Model</h3> \endhtmlonly
 ** 
 ** The membrane voltage \f$V_m\f$ is governed by 
 ** \f[
 ** C_m \frac{V_m}{dt} = -\frac{V_m-E_m}{R_m} - \sum_{c=1}^{N_c} g_c(t) ( V_m - E_{rev}^c ) + \sum_{s=1}^{N_s} I_s(t) + I_{inject}
 ** \f]
 ** with the following meanings of symbols
 ** - \f$C_m\f$ membrane capacity (Farad)
 ** - \f$E_m\f$ reversal potential of the leak current (Volts)
 ** - \f$R_m\f$ membrane resistance (Ohm)
 ** - \f$N_c\f$ total number of channels (active + synaptic)
 ** - \f$g_c(t)\f$ current conductance of channel \f$c\f$ (Siemens)
 ** - \f$E_{rev}^c(t)\f$ reversal potential of channel \f$c\f$ (Volts)
 ** - \f$N_s\f$ total number of current supplying synapses
 ** - \f$I_s(t)\f$  current supplied by synapse \f$s\f$ (Ampere)
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
 ** \f$V_{tresh}\f$ the MembranePatchSimple sends a spike to all its outgoing
 ** synapses.
 **
 ** The membrane voltage is reseted and clamped during the absolute
 ** refractory period of length \f$T_{refract}\f$ to \f$V_{reset}\f$
 ** if the flag \c doReset=1. This is similar to a LIF neuron (see
 ** LifNeuron).
 **
 ** If the flag \c doReset=0 the membrane voltage is not reseted and
 ** the above equation is also applied during the absolute refractory
 ** period but the event of threshold crossing is transmitted as a
 ** spike to outgoing synapses. This is usfull if one includes
 ** channels which produce a \em real action potential (see
 ** HH_K_Channel and HH_Na_Channel ) but one still just wants to
 ** communicate the spikes as events in time.
 **
 ** \latexonly  \subsubsection*{Implementation} \endlatexonly
 ** \htmlonly  <h3>Implementation</h3> \endhtmlonly
 **
 ** The exponential Euler method is used for numerical integration.
 **
 ***/
class MembranePatchSimple {

 public:

  MembranePatchSimple(void);

  virtual ~MembranePatchSimple();

  //! Reset the MembranePatchSimple.
  virtual void reset(void);

  //! Calculate the new membrane voltage and check wheter Vm exceeds the threshold V_thresh.
  virtual void IandGtot(double *i, double *g);

  //! The membrane capacity \f$C_m\f$ [range=(0,1); units=F;]
  float Cm;

  //! The membrane resistance \f$R_m\f$ [units=Ohm; range=(0,1e30)]
  float Rm;

  //! The reversal potential of the leakage channel [readonly; units=V; range=(-1,1)]
  double Em;

  //! The resting membrane voltage. [units=V; range=(-1,1);]
  /** The value of \f$E_m\f$ is calculated to compensate for ionic
   ** currents such that \f$V_m\f$ actually has a resting value of
   ** \f$V_\mathit{resting}\f$. This is done in reset().
   ** */
  float Vresting;

  //! Initial condition for\f$V_m\f$ at time \f$t=0\f$. [units=V; range=(-1,1);]
  float Vinit;

  //! Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev. [units=V; range=(0,1e5);]
  float VmScale;

  //! The membrane voltage [readonly; units=V;]
  double Vm;

  //! Variance of the noise to be added each integration time constant. [range=(0,1); units=W^2;]
  float Inoise;

  //! Constant current to be injected into the CB neuron. [units=A; range=(-1,1);]
  float Iinject;

 protected:
  friend class IonChannel;
  friend class ActiveChannel;

  //! Number of channels [readonly; units=;]
  int nChannels;

  //! Length of list of channels [hidden]
  int lChannels;

  //! W list of associated channels [hidden]
  IonChannel **channels;

  //! Call to add a new channel
  virtual void addChannel(IonChannel *newChannel);

  //! Call to destroy the channels
  virtual void destroyChannels(void);

};

#endif
