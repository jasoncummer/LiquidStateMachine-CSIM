/*! \file cbneuron.h
**  \brief Class definition of CbNeuron
*/

#ifndef _CBNEURON_H_
#define _CBNEURON_H_

#include "spikingneuron.h"
#include "ionchannel.h"
#include "membranepatch.h"

//! A single compartment neuron with an arbitrary number of channels, conductance based, as well as current based synapses.
/** 
 ** \latexonly \subsubsection*{The Model} \endlatexonly
 ** \htmlonly <h3>Model</h3> \endhtmlonly
 ** 
 ** The membrane voltage \f$V_m\f$ is governed by 
 ** \f[
 ** C_m \frac{V_m}{dt} = -\frac{V_m-E_m}{R_m} - \sum_{c=1}^{N_c} g_c(t) ( V_m - E_{rev}^c ) + \sum_{s=1}^{N_s} I_s(t) + \sum_{s=1}^{G_s} g_s(t)(V_m-E_{rev}^{(s)}) + I_{inject}
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
 ** - \f$G_s\f$ total number of conductance based synapses
 ** - \f$g_s(t)\f$ coductance supplied by synapse \f$s\f$ (Siemens)
 ** - \f$E_{rev}^{(s)}\f$ reversal potential of synapse \f$s\f$ (Volts)
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
 ** \f$V_{tresh}\f$ the CbNeuron sends a spike to all its outgoing
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
 ** HH_K_Channel and HH_Na_Channel) but one still just wants to
 ** communicate the spikes as events in time.
 **
 ** \latexonly  \subsubsection*{Implementation} \endlatexonly
 ** \htmlonly  <h3>Implementation</h3> \endhtmlonly
 **
 ** The flag \c nummethod is set to 0 the exponential Euler method 
 ** is used for numerical integration, otherwise the Crank-Nicolson.
 ** method.
 ***/

class CbNeuron : public SpikingNeuron, public MembranePatch {

 DO_REGISTERING  

 public:

  CbNeuron(void);
  virtual ~CbNeuron();

  //! Reset the CbNeuron.
  void reset(void);

  //! Calculate the new membrane voltage and check wheter Vm exceeds the threshold V_thresh.
  virtual double nextstate(void);

  //! Returns 1 (0) if the neuron is (not) in its absolute refractory period.
  virtual int isRefractory(void);

  //! If \f$V_m\f$ exceeds \f$V_{thresh}\f$ a spike is emmited. [units=V; range=(-10,100);]
  float Vthresh;

  //! The voltage to reset \f$V_m\f$ to after a spike. [units=V; range=(-1,1);]
  float Vreset;

  //! Flag which determines wheter \f$V_m\f$ should be reseted after a spike [units=flag; range=(0,1);];
  int doReset;

  //! Length of the absolute refractory period. [units=sec; range=(0,1);]
  float Trefract;

  //! Add an incoming channels
  virtual int addIncoming(Advancable *s);

  //! Add an outgoing channels
  virtual int addOutgoing(Advancable *s);

  //! Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1[units=flag; range=(0,1);];
  int nummethod;

 protected:

 //! synaptic input current
  double Isyn;

 //! synaptic input conductance
  double Gsyn;

 private:

  friend class CbNeuronSt;
  friend class CbStOuNeuron;
  friend class CbHHOuNeuron;
  friend class CbHHOuINeuron;
  friend class StaticAnalogCbSynapse;   // for access to GSummationPoint
  friend class DynamicSpikingCbSynapse; // for access to GSummationPoint
  friend class StaticSpikingCbSynapse;  // for access to GSummationPoint
  
  //! If positive then this counter tells us how many time steps we are still in the absolute refractory period
  int nStepsInRefr;

  //! Internal constants for the solution of the differential equation
  double C1;

  //! At this point all synaptic conductances are summed up \internal [hidden;]
  double GSummationPoint;

  //! W temporal place to store wether we want to spike or not.
  bool spike;

};

#endif
