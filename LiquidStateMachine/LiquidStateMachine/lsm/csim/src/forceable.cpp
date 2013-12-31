/*! \file forceable.cpp
**  \brief Implementation of class Force (is empty).
*/

#include "forceable.h"
#include "csimerror.h"
#include "teacher.h"

int Forceable::addIncoming(Advancable *potentialTeacher) 
{
  if ( myTeacher ) {
    TheCsimError.add("Forcable::addIncoming: object Forcable %i is already connected to Teacher %i",
		     getId(),myTeacher->getId());
    return -1;
  } 
  if ( !(myTeacher=dynamic_cast<Teacher *>(potentialTeacher)) ) {
    TheCsimError.add("Forcable::addIncoming: A %s is not a valid input for object %i [Forcable]",
		     potentialTeacher->className(),getId());
    return -1;
  }
  return 0;
};

//  int Forceable::addOutgoing(Advancable *A) 
//  {
//  }
