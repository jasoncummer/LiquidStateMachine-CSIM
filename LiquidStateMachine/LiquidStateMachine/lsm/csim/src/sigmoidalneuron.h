/*! \file sigmoidalneuron.h
**  \brief Class definition of SigmoidalNeuron
*/

#ifndef _SIGMOIDALNEURON_H_
#define _SIGMOIDALNEURON_H_

#include "analogneuron.h"

//! An analog neuron with a sigmoidal activation function
class SigmoidalNeuron : public AnalogNeuron {
  friend class ExtOutSigmoidalNeuron;

  DO_REGISTERING

 public:
  SigmoidalNeuron(void);

  //! Itot = logsig(beta*(Itot-thresh)) [readwrite; units=W;]
  double thresh;
  //! Itot = logsig(beta*(Itot-thresh)) [readwrite; units=1;]
  double beta;
  //! time constant [readwrite;]
  double tau_m;
  //! the output of a sig neuron is scaled to (0,W_max) [readwrite; units=1;]
  double A_max;
  //! external current injected into neuron [readwrite; units=W;]
  double I_inject;
  //! initial 'membrane voltage' [readwrite; units=V;]
  double Vm_init;

  virtual void reset(void);
  virtual double nextstate(void);

 private:
  //! constant for exponential Euler integration of Vm [hidden;]
  double C1;
  //! constant for exponential Euler integration of Vm [hidden;]
  double C2;

};

#endif
