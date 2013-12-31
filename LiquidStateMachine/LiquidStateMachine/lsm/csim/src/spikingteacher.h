/*! \file spikingteacher.h
**  \brief Class definition of SpikingTeacher
*/

#ifndef _SPIKINGTEACHER_H_
#define _SPIKINGTEACHER_H_

#include "teacher.h"
#include "spikingneuron.h"
#include "csiminputclass.h"


//! Teacher for a pool of spiking neurons

class SpikingTeacher : public Teacher, public csimSpikingInputClass {

  DO_REGISTERING

 public:

  virtual int advance();

  virtual int addOutgoing(Advancable *a);

};

#endif
