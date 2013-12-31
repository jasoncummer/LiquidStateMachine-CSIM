/*! \file hh_neuron.h
**  \brief Class definition of HHNeuron
*/

#ifndef _HH_NEURON_H_
#define _HH_NEURON_H_

#include "cbneuron.h"

class HH_K_Channel;
class HH_Na_Channel;

//! Conductance based spiking neuron using the HH squid modell.
/**
 ** The model is based on a CbNeuron and includes the HH_K_Channel and
 ** HH_Na_Channel for action potetial generation.
 **
 ** */

class HHNeuron : public CbNeuron {

 DO_REGISTERING  

 public:

  HHNeuron(void);

  virtual ~HHNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);

private:

  HH_K_Channel  *k;

  HH_Na_Channel *na;


};

#endif
