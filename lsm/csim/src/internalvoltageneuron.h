#ifndef __INTERNALVOLTAGENEURON_H__
#define __INTERNALVOLTAGENEURON_H__

#include "csimclass.h"

//! Base class for all neuron classes with membrane voltage
class InternalVoltageNeuron {
  
 protected:
  //InternalVoltageNeuron(void) { Vm = 0;};
  friend class GlutamateSynapse; // each synapse which accesses Vm needs to be a friend

  //! The membrane voltage \f$V_m\f$ [readonly; units=V;]
  double Vm;
  //double getVm(void) { return Vm;};
};

#endif

