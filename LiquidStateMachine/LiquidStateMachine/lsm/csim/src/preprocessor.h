/** \file preprocessor.h
**  \brief Base class of all preprocessors that can be applied to the filtered input of a readout.
*/

#ifndef _PREPROCESSOR_H_
#define _PREPROCESSOR_H_

#include <string>
#include <map>
#include <list>
#include "csimerror.h"
#include "csimclass.h"
#include "advanceable.h"

using namespace std;

//!  Base class of all preprocessors that can be applied to the filtered input of a readout. Examples are normalizers, PCA and linear transformations.
class Preprocessor : public Advancable {

 public:
  /** Constructs a new preprocessor object.
      \param in_rows Number of rows in each input vector.
      \param out_rows Number of rows in each output vector. */
  Preprocessor(unsigned int in_rows, unsigned int out_rows);

  virtual ~Preprocessor(void) {};

  /** Sets a parameter of the preprocessor function.
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


  /** Preprocess a state representation.
      \param S State of the liquid (= filtered response of the neural microcircuit).
      \param X Target vector where to save the results. 
      \return -1 if an error occured, 1 for success. */
  virtual int process(const double* S, double* X) = 0;

  /** Resets the information stored within the preprocessor. */
  virtual void reset() = 0;

  /** Imports the data from an externally (e.g. Matlab) trained preprocessor.
      \param rep Representation of the preprocessor as a double vector.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  virtual int importRepresentation(const double* rep, int rep_length) = 0;

  /** Exports the representation of this preprocessor for use in external objects.
      \param rep_length Length of the representation vector.
      \return A list of parameters that represent the preprocessor.
      \warning Do not forget to free the memory reserved for the representation! */
  virtual double* exportRepresentation(int *rep_length) = 0;

  /** Returns the number of input rows. */
  int getInputRows() { return nInputRows; }

  /** Returns the number of output rows. */
  int getOutputRows() { return nOutputRows; }

  /** Returns a textual description of the representation format for import/export - Representation. */
  virtual string getFormatDescription() = 0;

  /** This function is called after parameters are updated. */
  virtual int updateInternal();

 protected:

  //! A map storing pointers to the parameters. \internal [hidden;]
  map<string, double *> params;

  //! Number of rows for input vectors.
  int nInputRows;
  //! Dummy for parameter setting \internal [hidden;]
  double dInputRows;

  //! Number of rows for output vectors.
  int nOutputRows;
  //! Dummy for parameter setting \internal [hidden;]
  double dOutputRows;
};

#endif
