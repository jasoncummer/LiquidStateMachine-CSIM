/*! \file cbhhouineuron.h
**  \brief Implementation of conductance based neurons with Ornstein Uhlenbeck process noise and Hodgkin-Huxley spike generation mechanism
*/

#ifndef _CB_HH_OU_I_NEURON_H_
#define _CB_HH_OU_I_NEURON_H_

#include "cbneuron.h"
#include "spikingneuron.h"
#include "ionchannel.h"

class KDChannel_Traub91;
class NAChannel_Traub91;

//! A single compartment neuron with an arbitrary number of channels, current supplying synapses and spike template.
/** Missing!
 ***/
class CbHHOuINeuron : public CbNeuron {

 DO_REGISTERING

 public:

  CbHHOuINeuron(void);
  virtual ~CbHHOuINeuron();

  virtual double nextstate(void);

  virtual void reset(void);

  virtual int init(Advancable *a);

  virtual int updateInternal(void);

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

  KDChannel_Traub91  *k;

  NAChannel_Traub91 *na;

};

#endif
