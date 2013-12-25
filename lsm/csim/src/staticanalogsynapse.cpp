/*! \file staticanalogsynapse.cpp
**  \brief Implementation of StaticAnalogSynapse
*/

#include "staticanalogsynapse.h"
#include "randgen.h"
#include "stdio.h"

StaticAnalogSynapse::StaticAnalogSynapse(void)
{
  summationPoint = 0;
  psr = psi = 0;
  W = 1;
}

void StaticAnalogSynapse::reset(void)
{
/* ************************ BEGIN MICHAEL PFEIFFER *********************** */
  
  if ( Inoise > 0.0 )
    psr = psi * W + normrnd() * Inoise;
  else
    psr = psi * W;
  
  if (summationPoint) {
    (*summationPoint) += psr;
  }
  else {
    csimPrintf("StaticAnalogSynapse::reset Synapse %d is not connected to a neuron!\n",getId()); 
  }


  //  advance();
/* ************************* END MICHAEL PFEIFFER *********************** */
}

int StaticAnalogSynapse::advance(void)
{
  if ( Inoise > 0.0 )
    psr = psi * W + normrnd() * Inoise;
  else
    psr = psi * W;
  
  (*summationPoint) += psr;


  return 1;
}
