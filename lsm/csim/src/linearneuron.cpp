/*! \file linearneuron.cpp
**  \brief Implementation of LinearNeuron
*/

#include "randgen.h"
#include "linearneuron.h"

LinearNeuron::LinearNeuron(void) :
  Iinject(0.0), Vinit(0.0)
{

}

double LinearNeuron::nextstate(void)
{
  //  int i;
  // double Itot;

  // Here we integrate all
  //   - the postsynaptic currents,
  //   - the noise and the
  //   - injected current

  // Itot = Iinject;
	summationPoint += Iinject;

  if ( Inoise > 0.0 )
    summationPoint += normrnd()*Inoise;

  //for(i=0;i<nIncoming;i++) {
  //  Itot += (incoming[i]->psr);
  //}

  //Vm = Itot;
  VmOut = Vm = summationPoint+Vresting;
  summationPoint = 0;

  return Vm;
}

void LinearNeuron::reset(void)
{
  AnalogNeuron::reset();
  VmOut=Vm = Vinit;                   // set unit output to its initial value
  summationPoint = 0;
}
