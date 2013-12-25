/*! \file analogsynapse.cpp
**  \brief Implementation of AnalogSynapse
*/

#include "analogsynapse.h"

AnalogSynapse::AnalogSynapse(void)
{
  Inoise = 0;
  W = 1;
  delay = 0;
  psr = 0;
  delayIndex = 0;
  psi = 0;
}
