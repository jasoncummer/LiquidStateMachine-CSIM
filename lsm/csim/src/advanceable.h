/*! \file advanceable.h
**  \brief Class definition of Advancable
*/

#ifndef _ADVANCEABLE_H_
#define _ADVANCEABLE_H_

#include "csimclass.h"

//! Base class for all objects to simulate.
/**
 ** \model
 **
 ** Advancable defines the (virtual) methods advance() and reset()
 ** which must be implemented by any class in the simulation in order
 ** to implement a specific model.
 **
 ** At the beginning of a simulation (at time \f$t=0\f$) reset()
 ** iscalled. For each time step (of fixed length \f$\Delta t\f$)
 ** duringthe simulation advance() will be called for each object and
 ** it is assumed that during the call to advance() the object
 ** calculates its next state and sends the result of this computation
 ** to any destination/succesors/outgoing object.
 **
 ** \endmodel
 **
 ** \inout
 ** 
 ** Each object in the simulation gets input signals (analog or
 ** spikes) from \e source or \e incoming objects and sends output
 ** signals (analog or spikes) to \e destination or \e outgoing
 ** objects. Hence there are the two methods addIncoming() and
 ** addOutgoing() which will be called to set up the inward and
 ** outward signal flow. Within these methods each object (derived
 ** from Advancable) should check whether an object is a proper source
 ** (destination) of input (output) signals.
 **
 ** \endinout
 **
 **
 ** */
class Advancable : public csimClass {

 public:

  virtual ~Advancable(void){};


  //! Called at the beginning of a simulation at t=0.
  virtual void reset(void)=0;

  //! Calculate the next state and output/propagate the result to succesors/outgoing objects.
	//! The return value determines whether the advanceable object should be further advanced in the next
	//! step (return value 1) or can be inactived (return value 0) (Currently only SpikingSynapses can return 0).
  virtual int advance(void)=0;

  //! This method will be called if object \a Incoming wants to send information to \a this object
  virtual int addIncoming(Advancable *Incoming)=0;

  //! This method will be called if \a this object wants to send information to object Outgoing
  virtual int addOutgoing(Advancable *Outgoing)=0;

  inline char threadId() { return m_threadId; };

private:
  friend class Network;
  char m_threadId;

};

#endif

