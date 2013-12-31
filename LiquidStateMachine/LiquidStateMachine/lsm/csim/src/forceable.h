/*! \file forceable.h
**  \brief Class definition of Forcable 
*/

#ifndef _FORCEABLE_H_
#define _FORCEABLE_H_

#include "advanceable.h"

class Teacher;

//! Base class for all objects we can be forced (by some teacher; see class Teacher) to produce a given target output
/**
 
<b>Forcing an object</b> To be able to teach/force an object in a
proper way we decided to split the two steps (calculate next state and
output the result) which are usually done within the advance() method
into two explicit pieces: the class Forceable introduces the two
methods nextstate() and output() which represent this two steps. In
addition the method force() allows a potential teacher (see class
Teacher) to intervene and to force the object to give a certain
output.

<b>Implementing foreable objects</b> Each object derived from
Forceable must implement the three methods nextstate(), force() and
output(). One \e must \e not implement advance()! This is hardcoded as
a call to nextstate(); directly followd by output().  nextstate()
shoud perform the usual numeric calculations/integration. It should
store the result for further usage by output(). The call to output()
should then propagate the results to the outgoing objects.

<b>Active teacher</b> If teacher forcing is active a teacher will call
force() \e between the calls to nextstate() and output() and is thus
able to force acertain output (and also to overwrite any results
computed by advance()). However for efficiency reasons it is left to
the teacher if a call to nextstate() is necessary at all (since the
result may be overwritten) and it may well be the case the during
simulations with an active teacher nextstate() does not get called.

<b>Inactive teacher</b> During each simulation step advance() gets
called which in turn calls  nextstate() and  \a output.

\sa Teacher, SpikingTeacher, AnalogTeacher, SpikingSRCTeacher

*/
class Forceable : public  Advancable {

 public:

  //! The constructor ...
  Forceable(void) { myTeacher = 0; }
  virtual ~Forceable(void){};

  //! Calculate the next state of the object but <b>do not send</b> it to outgoing objects
  virtual double nextstate(void)=0;

  //! Allows some teacher to force a certain output or to overwrite what was computed during the call of nextstate().
  virtual void force(double y)=0;

  //! W call to output should actually promotes the output (actual or teacher) to the outgoing objects.
  virtual void output(void)=0;

  //! Advance of forcable objects is the sequence: nextstate(); output(); return 1;
  inline int advance(void) { nextstate(); output(); return 1; }

  //! If \c potentialTeacher is actually a teach then we will store that pointer (in \c myTeacher)
  virtual int addIncoming(Advancable *potentialTeacher);

  //! The teacher who will teach me (NULL for no teacher).
  Teacher *myTeacher;

};

#endif
