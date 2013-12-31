#ifndef __ACTIVECACHANEL_H__
#define __ACTIVECACHANEL_H__

#include "activechannel.h"

class IonGate;

//! Ion channel that contributes to the intracellular calcium concentration
// (Channel type will be recognized by CbNeuronStCa)
class ActiveCaChannel : public ActiveChannel {

  DO_REGISTERING

public:

  ActiveCaChannel(void) {};
  virtual ~ActiveCaChannel(void) {};

};

#endif
