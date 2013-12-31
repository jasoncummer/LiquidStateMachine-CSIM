/*! \file mexnetwork.h
**  \brief Class definition of MexNetwork
*/

#ifndef _MEXNETWORK_H_
#define _MEXNETWORK_H_

#include <mex.h>
#include "globaldefinitions.h"
#include "network.h"

class MexRecorder;

//! In addition to the class Network it provides specific methods for the Matlab MEX interface.
class MexNetwork : public Network {

public:
  
  //! Constructor 
  MexNetwork();
  
  //! Destructor
  virtual ~MexNetwork();
  
  //! Add a new object to the network. Note that we <b>do not check</b> if it was already added before.
  virtual int addNewObject(Advancable *O);

  //! Returns the output as a cell array for use in matlab
  mxArray *getMexOutput(void);

  //! Returns an mxArray which holds the parameters and connectivity of the whole network
  mxArray *exportNetwork(void);

  //! Builds a whole network based on the description in \c mxNet.
  /** Builds a whole network based on the description in \c mxNet.
   ** \param mxNet is a Matlab array (usually) created by exportNetwork(). 
   **
   **  Can only be called if there are no other objects in the network,
   **  i.e. one can not merge to network descriptions. */
  int importNetwork(const mxArray *mxNet);

private:
  csimList<MexRecorder,2> mexRecorderList;

};

#endif
