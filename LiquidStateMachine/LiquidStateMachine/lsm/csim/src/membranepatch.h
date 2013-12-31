/*! \file membranepatch.h
**  \brief Class definition of MembranePatch
*/

#ifndef _MEMBRANEPATCH_H_
#define _MEMBRANEPATCH_H_

#include "spikingneuron.h"
#include "ionchannel.h"
#include "synapsetarget.h"
#include "membranepatchsimple.h"
#include "ionbuffer.h"

class MembranePatchSimple;

#define CA 0     // first ion buffer is calcium

// To install a new buffer
// 1) create new active channel type, e.g. ActiveCaChannel (to be recognized by the membranepatch)
// 2) Call new and addBuffers in constructor of membranepatch (so buffer is added to membrane)
// 3) define new channel number, e.g. see above CA 0
// 4) write code to recognize new channel type in the addChannel methode of membrane patch


//! A a patch of membrane with an arbitrary number of channels and current supplying synapses
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
 Manual and Class Reference**
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
 ** \f$V_{tresh}\f$ the MembranePatch sends a spike to all its outgoing
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
class MembranePatch : public MembranePatchSimple  {

 public:

  MembranePatch(void);

  virtual ~MembranePatch();

  //! Calculate the new membrane voltage and check whether Vm exceeds the threshold V_thresh.
  virtual void IandGtot(double *i, double *g);

  //! Reset the MembranePatch.
  virtual void reset(void);

 protected:
  friend class ConcIonGate;

  //! Number of ion buffers [readonly; units=;]
  int nBuffers;

  //! Length of list of ion buffers [hidden]
  int lBuffers;

  //! A list of associated ion buffers [hidden]
  IonBuffer **buffers;

  //! Call to add a new ion buffer
  void addBuffer(IonBuffer *newBuffer);

  //! Call to add a new ion channel
  virtual void addChannel(IonChannel *c);


};

#endif
