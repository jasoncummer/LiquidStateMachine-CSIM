/*! \file extoutsigmoidalneuron.h
**  \brief Class definition of ExtOutSigmoidalNeuron
*/

#ifndef _EXTOUTSIGMOIDALNEURON_H_
#define _EXTOUTSIGMOIDALNEURON_H_

#include "sigmoidalneuron.h"

//! A sigmoidal neuron which writes its output to some external program
class ExtOutSigmoidalNeuron : public SigmoidalNeuron {

  DO_REGISTERING

  public:
  ExtOutSigmoidalNeuron(void);
  ~ExtOutSigmoidalNeuron();

  void output(void);
  void reset(void);

 private:
  //! ID of external input (is equal index of array) [readwrite;]
  int myIndex;

  //! Flag if external input is used or normal input [readwrite;]
  int beReal;
};

#endif
