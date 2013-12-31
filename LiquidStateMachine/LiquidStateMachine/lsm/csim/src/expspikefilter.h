/** \file expspikefilter.h
**  \brief Filter which simulates exponential decay of spikes.
*/

#ifndef _EXPSPIKEFILTER_H_
#define _EXPSPIKEFILTER_H_

#include "spikefilter.h"

using namespace std;

//!  Filter which simulates exponential decay of spikes.
class ExpSpikeFilter : public SpikeFilter {

  DO_REGISTERING

 public:
  /** Constructs a new exponential-decay filter.
      \param tau1 Time constant (default \f$tau_1 = 0.03\f$). Decay by \f$exp(-DT/\tau_1)\f$. */
  ExpSpikeFilter(double tau1 = 30e-3);

  ~ExpSpikeFilter(void) {};

  /** Updates the internal state of the filter. */
  int updateInternal();

  //! Time constant.
  double m_tau1;

 protected:
  /** Process the occurence of a spike. 
      \param channel Index of the channel to filter.
      \param target Target value where to store the new filtered value. */
  double processSpike(int channel, double *target);


  //! Decay constant. \internal [hidden;]
  double m_decay;


};

#endif
