/*! \file analogsynapse.h
**  \brief Class definition of AnalogSynapse
*/

#ifndef _ANALOGSYNAPSE_H_
#define _ANALOGSYNAPSE_H_

#include "synapse.h"

//! Base class of all analog synapses
class AnalogSynapse : public Synapse {
  friend class AnalogNeuron;
  friend class StaticAnalogSynapse;
  friend class DynamicAnalogSynapse;
  /* **********************
     BEGIN MICHAEL PFEIFFER
     ***********************/
  friend class PhysicalModel;
  /* ********************
     END MICHAEL PFEIFFER
     *********************/

 public:
  AnalogSynapse(void);

  //! The noise of our analog synapses [units=;]
  float Inoise;

  //! Returns the post-synaptic current of the synapse.
  //  virtual double psc(void);
  virtual int getIndex(void) { return delayIndex; }
  virtual void setPsi(double myVal) { psi=myVal; }

 protected:
  //! Index into queue of presynaptic neuron \internal [hidden;]
  // is filled out by reset() of presyn neuron
  int delayIndex;

  //! pre-synaptic analog input into this synapse \internal [hidden;]
  // this comes from preNeuron::output()
  double psi;


};

#endif
