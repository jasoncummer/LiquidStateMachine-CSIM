/*! \file  extinputneuron.h
**  \brief Interface of ExtInputNeuron
*/
#ifndef _EXTINPNEURON_H_
#define _EXTINPNEURON_H_

#include "neuron.h"
#include "analoginputneuron.h"
#include "globaldefinitions.h"

//! Implements an external (analog) input neuron
class ExtInputNeuron : public AnalogInputNeuron {

  DO_REGISTERING

 public:
  ExtInputNeuron(void);
  ~ExtInputNeuron();

  virtual void reset(void);
  virtual int addIncoming(Advancable *a);
  virtual double nextstate(void);

 private:
  //! ID of external input (is equal index of array) [readwrite;]
  int myIndex;

  //! Flag if external input is used or normal input [readwrite;]
  int beReal;
};

#endif
