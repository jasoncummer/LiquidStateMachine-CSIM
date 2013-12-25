/** \file algorithm.h
**  \brief Base class of all learning algorithms that calculate the readout's output.
*/

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <string>
#include <map>
#include <list>
#include "csimerror.h"
#include "csimclass.h"
#include "advanceable.h"

using namespace std;

//! Base class of all learning algorithms that calculate the readout's output. Algorithms take the filtered and preprocessed input of a readout and calculate one single output. 
//! The current algorithm interface is designed for offline-training only, i.e. all learning must occur externally. The algorithm object only "learns" through imported parameter vectors.
class Algorithm : public Advancable {

 public:
  /** Constructs a new learning algorithm.
      \param in_rows Number of rows in each input vector. 
      \param lower_bound Lower bound of the algorithms range. 
      \param upper_bound Upper bound of the algorithms range. */
  Algorithm(unsigned int in_rows=1, double lower_bound=0, double upper_bound=1);

  virtual ~Algorithm(void) {};

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


  /** Sets a parameter of the algorithm.
      \param name Name of the parameter.
      \param value Value to set for the parameter. */
  virtual void setParameter(string name, double value);


  /** Returns the current value of a parameter.
      \param name Name of the parameter.
      \return The value of the parameter. */
  virtual double getParameter(string name);

  /** Returns the names of the valid parameters.
      \return A string list indicating the valid parameter names. */
  virtual list<string> validParameters(void);


  /** Applies the currently learned function to the filtered and preprocessed input vector.
      \param S State of the liquid (= filtered and preprocessed response of the neural microcircuit).
      \param X Target pointer where to save the result. 
      \return -1 if an error occured, 1 for success. */
  virtual int apply(const double* S, double* X) = 0;

  /** Resets the information stored within the algorithm. */
  virtual void reset() {};

  /** Imports the data from an externally (e.g. Matlab) trained algorithm.
      \param rep Representation of the algorithm as a double vector.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  virtual int importRepresentation(const double* rep, int rep_length) = 0;

  /** Exports the representation of this algorithm for use in external objects.
      \param rep_length Length of the representation vector.
      \return A list of parameters that represent the algorithm.
      \warning Do not forget to free the memory reserved for the representation! */
  virtual double* exportRepresentation(int *rep_length) = 0;

  /** Returns the number of input rows. */
  int getInputRows() { return nInputRows; }

  /** Returns the range of the algorithm's target values [a, b].
      \param a Address of lower bound for algorithm's target values.
      \param b Address of upper bound for algorithm's target values. */
  void getRange(double* a, double *b);
  
  /** Sets the range of the algorithm's target values [a, b].
      \param a Lower bound for algorithm's target values.
      \param b Upper bound for algorithm's target values. */
  void setRange(double a, double b);
  


  /** Returns a textual description of the representation format for import/export - Representation. */
  virtual string getFormatDescription() = 0;

  /** This function is called after parameters are updated. */
  virtual int updateInternal();

  //! Lower bound of algorithms range.
  double range_low;

  //! Upper bound of algorithms range.
  double range_high;

 protected:

  //! A map storing pointers to the parameters. \internal [hidden;]
  map<string, double *> params;

  //! Number of rows for input vectors.
  int nInputRows;
  //! Dummy for parameter setting \internal [hidden;]
  double dInputRows;
};

#endif
