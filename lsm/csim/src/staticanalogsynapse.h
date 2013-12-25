/*! \file staticanalogsynapse.h
**  \brief Class definition of StaticAnalogSynapse
*/

#ifndef _STATICANALOGSYNAPSE_H_
#define _STATICANALOGSYNAPSE_H_

#include "analogsynapse.h"

//! A synapse which transmitts analog values (no dynamics)
class StaticAnalogSynapse : public AnalogSynapse {

 DO_REGISTERING 

 public:
  StaticAnalogSynapse(void);
   

  //! Reset to initial condition.  
  void reset(void);

  //! Advance the state of the Synapse
  int advance(void);


 private:

// The following must be included to be able to register this object

};

#endif
