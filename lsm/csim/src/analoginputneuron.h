/*! \file analoginputneuron.h
**  \brief Class definition of AnalogInputNeuron
*/

#ifndef _ANALOGINPUTNEURON_H_
#define _ANALOGINPUTNEURON_H_

#include "analogneuron.h"
#include "csiminputclass.h"

//! An object which outputs a predefined analog signal
class AnalogInputNeuron : public AnalogNeuron, public csimAnalogInputClass {
  
 DO_REGISTERING 

 public:
  AnalogInputNeuron(void);
  double nextstate(void);
  void reset(void);
  //  virtual int setInput(csimInput *);
  // virtual int unSetInput(void);
  virtual int addIncoming(Advancable *a);
  virtual int addOutgoing(Advancable *a);

};

#endif
