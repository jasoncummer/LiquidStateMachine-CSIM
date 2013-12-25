/*! \file analogteacher.cpp
**  \brief Implementation of AnalogTeacher
*/


#include <stdlib.h>
#include "analogteacher.h"
#include "csimerror.h"

int AnalogTeacher::advance() {

  double  value = nextValue(0);
  Forceable *f;

  for (unsigned i=0; i<forceableList.n; i++) {
    f=forceableList.elements[i];

    // first let elements calc their nextstate an potential output
    f->nextstate();

    // now we overwrite what they have are intenden to output 
    f->force(value);

    // Output what we have forced
    f->output();

  }

  return 1;
}

int AnalogTeacher::addOutgoing(Advancable *a) {
  AnalogNeuron *an = dynamic_cast<AnalogNeuron *>(a);
  if (!an) {
    TheCsimError.add("AnalogTeacher::addOutgoing: Trying to add a non-analog neuron to an analog teacher\n");
    return -1;
  }
  Teacher::addOutgoing(a);

  return 0;
}
