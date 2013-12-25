/*! \file specific_neurons.h
**  \brief Class definition of several specific conductance based neuron models
*/

#ifndef _SP_NEURON_H_
#define _SP_NEURON_H_

#include "cbneuronst.h"
#include "ahp_channel.h"
#include "specific_ion_channels.h"


//! Conductance based non-accomodating spiking neuron (with spike template)
/** Uses AHP_Channel */
class bNACNeuron : public CbNeuronSt {

 DO_REGISTERING

 public:

  bNACNeuron(void);

  virtual ~bNACNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);

private:

  AHP_Channel  *ahp;


};


//! Conductance based accomodating spiking neuron (with spike template)
/** Uses AHP_Channel */
class cACNeuron : public CbNeuronSt {

 DO_REGISTERING

 public:

  cACNeuron(void);

  virtual ~cACNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);

private:

  AHP_Channel  *ahp;


};


//! Conductance based non-accomodating spiking neuron (with spike template)
/** Uses AChannel_Hoffman97 */
class dNACNeuron : public CbNeuronSt {

 DO_REGISTERING

 public:

  dNACNeuron(void);

  virtual ~dNACNeuron();

  virtual int init(Advancable *a);

  virtual int updateInternal(void);


private:

  AChannel_Hoffman97  *ah;

};



#endif
