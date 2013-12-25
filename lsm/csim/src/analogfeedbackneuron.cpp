/*! \file analogfeedbackneuron.cpp
**  \brief Implementation of AnalogFeedbackNeuron
*/

#include "analogfeedbackneuron.h"
#include "csimerror.h"

AnalogFeedbackNeuron::AnalogFeedbackNeuron(void)
{
  feedback = EXTERNAL_INPUT;
}

double AnalogFeedbackNeuron::nextstate(void)
{
  if (feedback == EXTERNAL_INPUT) {
    // Take external input
    return (VmOut=Vm=nextValue(0)+Vresting);
  }
  else {
    // Receive internal feedback
    VmOut=Vm=summationPoint;
    summationPoint = 0;
    return VmOut;
  }
}

void AnalogFeedbackNeuron::reset(void)
{
  AnalogInputNeuron::reset();
}

int AnalogFeedbackNeuron::addIncoming(Advancable *a)
{
  Synapse *syn;
  if ( (syn=dynamic_cast<Synapse *>(a)) ) {
    // Make normal connection
    return Neuron::addIncoming(a);
  }

  // Other input (like readout) is allowed but not stored
  return 0;
}

int AnalogFeedbackNeuron::addOutgoing(Advancable *S)
{
  return AnalogInputNeuron::addOutgoing(S);
}


//! Toggles the feedback-mode of the neuron
int AnalogFeedbackNeuron::switchMode(int feedback_mode) {
  if ((feedback_mode == EXTERNAL_INPUT) || (feedback_mode == INTERNAL_FEEDBACK)) {
    feedback = feedback_mode;
  }
  else {
    TheCsimError.add("AnalogFeedbackNeuron::switchMode: Invalid feedback-mode %i!\n", feedback_mode);
    return -1;
  }
}


