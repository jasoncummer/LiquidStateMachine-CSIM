/*! \file teacher.cpp
**  \brief Implementation of Teacher
*/

#include "teacher.h"
#include "forceable.h"
#include "csimerror.h"
#include "network.h"

Teacher::~Teacher()
{
}

int Teacher::addOutgoing(Advancable *a)
{
  Forceable *f=dynamic_cast<Forceable *>(a);
  if ( f ) {
    if ( !f->myTeacher ) {
      forceableList.add(f);
      TheCurrentNetwork()->moveToTeached(f);
    } else {
      TheCsimError.add("Teacher::addOutgoing: object %s %i already taught by teacher %i!\n",
		       a->className(),a->getId(),f->myTeacher->getId());
      return -1;
    }
  } else {
    TheCsimError.add("Teacher::addOutgoing: object %s %i not a forcable object!\n",
		     a->className(),a->getId());
    return -1;
  }
  return 0;
}

//  int Teacher::addIncoming(Advancable *a)
//  {
//  }


