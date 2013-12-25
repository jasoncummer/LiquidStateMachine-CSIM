/*! \file dynamicanalogsynapse.h
**  \brief Class definition of DynamicAnalogSynapse
*/

#ifndef _DYNAMICANALOGSYNAPSE_H_
#define _DYNAMICANALOGSYNAPSE_H_

#include "analogsynapse.h"

//! Implements dynamic analog synapses
class DynamicAnalogSynapse : public AnalogSynapse {

  // DO_REGISTERING

 public:

  DynamicAnalogSynapse(void);

  //! Reset to initial condition.
  virtual void reset(void);

  //! Advance the state of the Synapse
  virtual int advance(void);

  //! The dynamic parameter for this synapse type
  float U,D,F;

 private:

  //! The exponential euler integration "constants"
  double Cd1, Cd2, Cf1, Cf2, d, f_bar;

// The following must be included to be able to register this object


};

#endif
