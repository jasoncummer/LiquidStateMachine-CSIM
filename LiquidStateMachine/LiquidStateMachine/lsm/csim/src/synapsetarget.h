#ifndef __SYNAPSETARGET_H__
#define __SYNAPSETARGET_H__

#include "csimclass.h"

//! Base class for all classes which are potential synaptic targets
class SynapseTarget {
  
protected:
  friend class Synapse;
  
  /* **********************
     BEGIN MICHAEL PFEIFFER
     ********************** */
  friend class Readout;
  /* ********************
     END MICHAEL PFEIFFER
     ******************** */


  // Needs to be reimplemented by each synaptic target
  // virtual uint32 getPostId(void)=0;

  //! At this point all synaptic input is summed up \internal [hidden;]
  double summationPoint;
  
};

#endif

