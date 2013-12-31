/*! \file linearneuron.h
**  \brief Class definition of LinearNeuron
*/

#ifndef _LINEARNEURON_H_
#define _LINEARNEURON_H_

#include "analogneuron.h"

//! A linear neuron: simply summing up the inputs.
class LinearNeuron : public AnalogNeuron {

  DO_REGISTERING

 public:
  LinearNeuron(void);

  //! external current injected into neuron [readwrite; units=W;]
  double Iinject;
  //! initial 'membrane voltage' [readwrite; units=V;]
  double Vinit;

  virtual void reset(void);
  virtual double nextstate(void);

};

#endif
