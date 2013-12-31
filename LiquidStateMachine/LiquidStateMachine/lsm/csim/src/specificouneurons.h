/*! \file specificouneurons.h
**  \brief Declartions of various conductance based neurons with Ornstein Uhlenbeck process noise
*/

#ifndef _SP_OU_NEURON_H_
#define _SP_OU_NEURON_H_

#include "cbstouneuron.h"
#include "ahp_channel.h"
#include "specific_ion_channels.h"


//! Conductance based non-accomodating spiking neuron with Ornstein Uhlenbeck process noise
/** Uses AHP_Channel
*/
class bNACOUNeuron : public CbStOuNeuron {

 DO_REGISTERING

 public:

  bNACOUNeuron(void);

  virtual ~bNACOUNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);

private:

  AHP_Channel  *ahp;


};


//! Conductance based accomodating spiking neuron with Ornstein Uhlenbeck process noise
/** Uses AHP_Channel
*/
class cACOUNeuron : public CbStOuNeuron {

 DO_REGISTERING

 public:

  cACOUNeuron(void);

  virtual ~cACOUNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);

private:

  AHP_Channel  *ahp;


};


//! Conductance based non-accomodating spiking neuron with Ornstein Uhlenbeck process noise
/** Uses AChannel_Hoffman97
*/
class dNACOUNeuron : public CbStOuNeuron {

 DO_REGISTERING

 public:

  dNACOUNeuron(void);

  virtual ~dNACOUNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);


private:

  AChannel_Hoffman97  *ah;

};



#endif
