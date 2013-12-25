/*! \file extoutlinearneuron.h
**  \brief Class definition of ExtOutLinearNeuron
*/

#ifndef _EXTOUTLINEARNEURON_H_
#define _EXTOUTLINEARNEURON_H_

#include "linearneuron.h"

//! A linear neuron which writes its output to some external program
class ExtOutLinearNeuron : public LinearNeuron {

  DO_REGISTERING

  public:
  ExtOutLinearNeuron(void);
  ~ExtOutLinearNeuron();

  void output(void);
  void reset(void);

 private:
  //! ID of external input (is equal index of array) [readwrite;]
  int myIndex;

  //! Flag if external input is used or normal input [readwrite;]
  int beReal;
};

#endif
