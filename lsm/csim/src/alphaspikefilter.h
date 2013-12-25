/** \file alphaspikefilter.h
**  \brief Filter which simulates spikes with alpha functions.
*/

#ifndef _ALPHASPIKEFILTER_H_
#define _ALPHASPIKEFILTER_H_

#include "spikefilter.h"

using namespace std;

//!  Filter which simulates spikes with alpha functions.
class AlphaSpikeFilter : public SpikeFilter {

  DO_REGISTERING

 public:
  /** Constructs a new alpha-function spike filter.
      \param tau1 Time constant (default \f$tau_1 = 0.03\f$). Decay by \f$alpha(-DT/\tau_1)\f$.
      \param tau2 Time constant (default \f$tau_1 = 0.003\f$). Decay by \f$alpha(-DT/\tau_2)\f$. */
  AlphaSpikeFilter(double tau1 = 30e-3, double tau2 = 3e-3);

  ~AlphaSpikeFilter(void) {};

  /** Updates the internal state of the filter. */
  int updateInternal();

  //! Time constant 1.
  double m_tau1;
  //! Time constant 2.
  double m_tau2;

 protected:
  /** Process the occurence of a spike. 
      \param channel Index of the channel to filter.
      \param target Target value where to store the new filtered value. */
  double processSpike(int channel, double *target);


  //! Decay constant 1. \internal [hidden;]
  double m_decay1;
  //! Decay constant 2. \internal [hidden;]
  double m_decay2;


};

#endif
