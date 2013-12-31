/*! \file dynamicanalogsynapse.cpp
**  \brief Implementation of DynamicAnalogSynapse
*/

#include "randgen.h"
#include "dynamicanalogsynapse.h"

DynamicAnalogSynapse::DynamicAnalogSynapse(void)
{
}

void DynamicAnalogSynapse::reset(void)
{
  double p;

  p = ( f_bar * (1-U) + U ) * d;
  psr = psi * p * W;

  if ( D > 0 ) {          // init const for exp. Euler integration
    Cd1 = exp(-DT/D);
  } else {
    Cd1 = 0.0;
  }
  // Cf1 and Cf2 are not constant !!!
}

int DynamicAnalogSynapse::advance(void)
{
  double p,fux;

  // calculate output of synapse with current dynamics
  p = ( f_bar * (1-U) + U ) * d;

  if ( Inoise > 0.0 )
    psr = psi * W * p + normrnd() * Inoise;
  else
    psr = psi * W * p;

  (*summationPoint) += psr;

  // consider next time step
  fux   = F * U * psi;
  Cf1   = exp ( -DT * ( U * psi + 1 / F ) );
  Cf2   = Cf1 * fux / ( 1 + fux );
  Cd2   = ( 1 - d * p * psi ) * ( 1 - Cd1 );
  f_bar = f_bar * Cf1 + Cf2;
  d     = d * Cd1 + Cd2;
  return 1;
}

