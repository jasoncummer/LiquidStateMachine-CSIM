#ifndef _TEACHER_H_
#define _TEACHER_H_

#include "advanceable.h"
#include "csimlist.h"

class Forceable;

//! Container class for a set of objects subject to teacher forcing
class Teacher : public Advancable { 

  public:

  Teacher(void) { };
  
  virtual void reset(void){};
  
  virtual ~Teacher();
  
  virtual int addOutgoing(Advancable *a);
  
  virtual int addIncoming(Advancable *) { return 0; };
  
protected:
  csimList<Forceable,100> forceableList;

};


#endif
