/** \file linear_classification.h
**  \brief Implementation of a linear classification.
*/

#ifndef _LINEAR_CLASSIFICATION_H_
#define _LINEAR_CLASSIFICATION_H_

#include "algorithm.h"

using namespace std;

//! Implementation of a linear classification.
class linear_classification : public Algorithm {

  DO_REGISTERING

 public:
  /** Constructs a new linear classification algorithm.
      \param in_rows Number of rows in each input vector.
      \param classes Number of classes for classification task. */
  linear_classification(unsigned int in_rows = 1, unsigned int classes = 2);
  
  ~linear_classification(void);

  /** Applies the currently learned function to the filtered and preprocessed input vector.
      \param S State of the liquid (= filtered and preprocessed response of the neural microcircuit).
      \param X Target pointer where to save the result. 
      \return -1 if an error occured, 1 for success. */
  int apply(const double* S, double* X);

  /** Resets the information stored within the algorithm. */
  void reset();

  /** Imports the data from an externally (e.g. Matlab) trained algorithm.
      \param rep Representation of the algorithm as a double vector.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  int importRepresentation(const double* rep, int rep_length);

  /** Exports the representation of this algorithm for use in external objects.
      \param rep_length Length of the representation vector.
      \return A list of parameters that represent the algorithm.
      \warning Do not forget to free the memory reserved for the representation! */
  double* exportRepresentation(int *rep_length);

  /** Returns a textual description of the representation format for import/export - Representation. */
  string getFormatDescription();

  //! Number of Classes
  int nClasses;

  //! Add bias (yes / no)
  bool addBias;

 private:

  //! Vector of linear-classification coefficients.
  double **regression_coefficients;

  /** Calculate a weighted sum of input S and weights w. */
  double weighted_sum(const double* S, const double* w);
};

#endif
