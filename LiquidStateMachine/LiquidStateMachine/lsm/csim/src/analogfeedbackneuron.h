/*! \file analogfeedbackneuron.h
**  \brief Class definition of AnalogFeedbackNeuron
*/

#ifndef _ANALOGFEEDBACKNEURON_H_
#define _ANALOGFEEDBACKNEURON_H_

#include "analoginputneuron.h"

//! Constant for external input
#define EXTERNAL_INPUT 0
//! Constant for internal feedback
#define INTERNAL_FEEDBACK 1

//! An object which outputs a predefined analog signal or an analog feedback from a readout or physical model
class AnalogFeedbackNeuron : public AnalogInputNeuron {
  
  DO_REGISTERING 
    
    public:
  AnalogFeedbackNeuron(void);
  ~AnalogFeedbackNeuron() {
  }
  double nextstate(void);
  void reset(void);
  
  virtual int addIncoming(Advancable *a);
  virtual int addOutgoing(Advancable *a);

  //! Toggles the feedback-mode of the neuron
  virtual int switchMode(int feedback_mode);

  //! Feedback-Mode: 0 = external input, 1 = feedback [range=(0,1);]
  int feedback;
};

#endif
