/** \file filterfunction.h
**  \brief Abstract base class of all filter objects
*/

#ifndef _FILTERFUNCTION_H_
#define _FILTERFUNCTION_H_

#include <string>
#include <map>
#include <list>
#include "csimerror.h"
#include "advanceable.h"

using namespace std;

//! Base class of all filter objects
class FilterFunction : public Advancable {

 public:
  FilterFunction(void);

  virtual ~FilterFunction(void) {};

  /** Sets a parameter of the filter function.
      \param name Name of the parameter.
      \param value Value to set for the parameter. */
  virtual void setParameter(string name, double value);

  //! Dummy implementation
  virtual int advance(void) {
    return 1;
  }

  //! This method will be called if object \a Incoming wants to send information to \a this object
  virtual int addIncoming(Advancable *Incoming) {
    return 0;
  }

  //! This method will be called if \a this object wants to send information to object Outgoing
  virtual int addOutgoing(Advancable *Outgoing) {
    return 0;
  }


  /** Returns the current value of a parameter.
      \param name Name of the parameter.
      \return The value of the parameter. */
  virtual double getParameter(string name);

  /** Returns the names of the valid parameters.
      \return A string list indicating the valid parameter names. */
  virtual list<string> validParameters(void);


  /* Filter a response signal of the neural microcircuit.
      \param R Response of the neural microcircuit.
      \param X Target vector where to save the results. 
      \param indices Indices where to store the results in X.
      \return -1 if an error occured, 1 for success. */
  virtual int filter(const double* r, double* x, int* indices=0) = 0;


  /** Resets the information stored within the filter. */
  virtual void reset() = 0;


  /** This function is called after parameters are updated. */
  virtual int updateInternal() {return 0;};

 protected:

  /** A map storing pointers to the parameters. */
  map<string, double *> params;
};

#endif

