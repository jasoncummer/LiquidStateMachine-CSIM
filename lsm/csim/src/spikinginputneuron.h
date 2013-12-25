/*! \file spikinginputneuron.h
**  \brief Class definition of SpikingInputNeuron
*/

#ifndef _SPIKINGINPUTNEURON_H_
#define _SPIKINGINPUTNEURON_H_

#include "spikingneuron.h"
#include "csiminputclass.h"

//! A spiking neuron which emits a predefined spike train.
class SpikingInputNeuron : public SpikingNeuron, public csimSpikingInputClass {

  DO_REGISTERING 
  
 public:

  virtual void reset(void);

  virtual double nextstate(void);

  //! Overwritten bcs we do not want a SpikingInputNeuron to be teached!
  virtual void force(double ) { };

  //! Overwritten bcs we do not want a SpikingInputNeuron to be teached!
  virtual void output(void) { };

};

#endif
