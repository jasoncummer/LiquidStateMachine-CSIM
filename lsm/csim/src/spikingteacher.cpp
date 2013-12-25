/*! \file spikingteacher.cpp
**  \brief Implementation of SpikingTeacher
*/

#include "spikingteacher.h"
#include "csimerror.h"

int SpikingTeacher::advance() {

  double  spike = nextValue(0);
  Forceable *f;

  for (unsigned i=0; i<forceableList.n; i++) {
    f=forceableList.elements[i];

    // first let elements calc their nextstate an potential output
    f->nextstate();

    // now we overwrite what they have are intenden to output 
    f->force(spike);

    // Output what we have forced
    f->output();

  }

  return 1;
}
 
int SpikingTeacher::addOutgoing(Advancable *a) {
  SpikingNeuron *s = dynamic_cast<SpikingNeuron *>(a);
  if (!s) {
    TheCsimError.add("SpikingTeacher::add: trying to add a non-spiking neuron to a spiking teacher\n");
    return -1;
  }
  Teacher::addOutgoing(s);

  return 0;
}
