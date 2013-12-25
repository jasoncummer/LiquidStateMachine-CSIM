/*! \file analogteacher.h
**  \brief Class definition of AnalogTeacher
*/

#ifndef _ANALOGTEACHER_H_
#define _ANALOGTEACHER_H_

#include "teacher.h"
#include "analogneuron.h"
#include "csiminputclass.h"
#include <stdio.h>

//! Teacher for a pool of analog neurons (all teached with the same signal)
class AnalogTeacher : public Teacher, public csimAnalogInputClass { 

 DO_REGISTERING

 public:

  virtual int advance(void);

  virtual int addOutgoing(Advancable *a); // inherited from teacher.h

};

#endif
