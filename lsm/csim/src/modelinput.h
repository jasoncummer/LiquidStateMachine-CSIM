#ifndef __MODELINPUT_H__
#define __MODELINPUT_H__

#include "csimclass.h"

//! Base class for all classes which are potential inputs for physical models
class ModelInput {
  
protected:
  friend class PhysicalModel;

  //! Output variable of a readout
  double output;
};

#endif

