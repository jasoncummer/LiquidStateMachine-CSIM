/*! \file extoutlifneuron.h
**  \brief Class definition of ExtOutLifNeuron
*/

#ifndef _EXTOUTLIFNEURON_H_
#define _EXTOUTLIFNEURON_H_

#include "lifneuron.h"

//! A LIF neuron which writes its output to some external program
class ExtOutLifNeuron : public LifNeuron {

  DO_REGISTERING 

  public:
  ExtOutLifNeuron(void);
  ~ExtOutLifNeuron();

  void output(void);
  void reset(void);

  //! ID of external input (is equal index of array) [readwrite;]
  int myIndex;

  //! Flag if external input is used or normal input [readwrite;]
  int beReal;
};

#endif
