/*! \file stdpsynapse.h
**  \brief Class definition of StdpSynapse
*/

#ifndef _STDPSYNAPSE_H_
#define _STDPSYNAPSE_H_

#include "spikingsynapse.h"
#include "needspostspikesynapse.h"

//! Base class for all  spiking synapses with spike time dependent plasticity (STDP).
/** Implements the basic weight update for a time difference \f$Delta =
t_{post}-t_{pre}\f$ with presynaptic spike at time \f$t_{pre}\f$ and
postsynaptic spike at time \f$t_{post}\f$. Then, the weight update is given by
\f$dw =  Apos * exp(-Delta/taupos)\f$ for \f$Delta > 0\f$, and \f$dw =  Aneg *
exp(-Delta/tauneg)\f$ for \f$Delta < 0\f$. (set \f$useFroemkeDanSTDP=0\f$ and
\f$mupos=muneg=0\f$ for this basic update rule).

It is also possible to use an
extended multiplicative update by changing mupos and muneg. Then \f$dw =
(Wex-W)^{mupos} * Apos * exp(-Delta/taupos)\f$ for \f$Delta > 0\f$ and \f$dw =
W^{mupos} * Aneg * exp(Delta/tauneg)\f$ for \f$Delta < 0\f$. (see Guetig,
Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through
non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23.
pp.3697-3714.)

Set \f$useFroemkeDanSTDP=1\f$ (this is the default value) and
use \f$tauspost\f$ and \f$tauspre\f$ for the rule given in Froemke and Dan
(2002). Spike-timing-dependent synaptic modification induced by natural spike
trains. Nature 416 (3/2002). */
class StdpSynapse : public SpikingSynapse {
 public:

  //! The constructor ...
  StdpSynapse(void);

  // Called if the postsynaptic neuron emmits a spike
  // virtual void preSpikeNotify(int iNewSpike);

  //! Called if the postsynaptic spikes hits (arrives at) the synapse
  virtual int preSpikeHit(void);

  //! Called if the postsynaptic spikes hits (arrives at) the synapse (inherited from NeedsPostSpikeSynapse)
  virtual int postSpikeHit(void);

  //! connect the presynaptic neuron
  virtual int addIncoming(Advancable *a);

  //! connect the postsynaptic neuron
  virtual int addOutgoing(Advancable *a);

  //! The method which does the actual STDP learning, i.e. changes one (ore more) parameters of the synapse.
  virtual void stdpLearning(double delta, double epost, double epre);

  //! Defines the model (static, dynamic) how the PSR is increased if a spike hits the synapse
  virtual void stdpChangePSR(void){};

  //! The maximum value of \f$Delta = t_{post}-t_{pre}\f$ which should be considered for STDP. [units=sec; range=(0,+100);]
  // float Tmax;

  //! The minimum value of \f$Delta = t_{post}-t_{pre}\f$ which should be considered for STDP. [units=sec; range=(-100,0);]
  // float Tmin;

  //! Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay [units=sec]
  float back_delay;

  //! Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).
  float tauspost;
  //! Used for extended rule by Froemke and Dan
  float tauspre;

  //! Timeconstant of exponential decay of positive learning window for STDP
  float taupos;
  //! Timeconstant of exponential decay of negative learning window for STDP
  float tauneg;

  float dw;
  //! No learning is performed if \f$|Delta| = |t_{post}-t_{pre}| < STDPgap\f$ 
  float STDPgap;
  //! Set to 1 to activate STDP-learning. No learning is performed if set to 0
  int activeSTDP;
  //! activate extended rule by Froemke and Dan (default=1)
  int useFroemkeDanSTDP;

  //! The maximal/minimal weight of the synapse [readwrite; units=;]
  float Wex;

  //! Defines the peak of the negative exponential learning window.
  float Aneg;
  //! Defines the peak of the positive exponential learning window.
  float Apos;
  
  //! Extended multiplicative positive update: \f$dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)\f$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.
  float mupos;
  //! Extended multiplicative negative update: \f$dw = W^{mupos} * Aneg * exp(Delta/tauneg)\f$. Set to 0 for basic update.
  float muneg;

};

#endif
