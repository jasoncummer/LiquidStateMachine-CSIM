/*! \file staticanalogcbsynapse.h
**  \brief Implementation of conductance based StaticAnalogSynapse
*/

#ifndef _STATICANALOGCBSYNAPSE_H_
#define _STATICANALOGCBSYNAPSE_H_

#include "analogsynapse.h"
#include "lifneuron.h"

//! A conductance based synapse which transmitts analog values. 
//! Its input is the synaptic conductance and its output is the PSC.
class StaticAnalogCbSynapse : public AnalogSynapse {

 DO_REGISTERING 

 public:
  StaticAnalogCbSynapse(void);

  //! Reset to initial condition.  
  void reset(void);

  //! Advance the state of the Synapse
  int advance(void);

  //! Outgoing objects must be CbNeurons
  virtual int addOutgoing(Advancable *a);

  //! Equilibrium potential given by the Nernst equation.
  double E;

 private:

  //! Pointer to postsynaptic conductance summation point within the \link SynapseTarget synaptic target \endlink ( Neuron, SynapticChannel, Compartment...) \internal [hidden;]
  double *GSummationPoint;

};

#endif
