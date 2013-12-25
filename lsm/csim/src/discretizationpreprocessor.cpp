/** \file discretizationpreprocessor.cpp
**  \brief Implementation of a discretization of the input.
*/

#include "discretizationpreprocessor.h"
#include <math.h>

/** Constructs a new discretization. Initially the transformation is an identical transformation x'=x.
    \param rows Number of rows in each input vector. */
DiscretizationPreprocessor::DiscretizationPreprocessor(unsigned int rows) : Preprocessor(rows, rows) {

  // Initialize to identical transformation
  epsilon = (double *) calloc(rows, sizeof(double));
  for (int i=0; i<rows; i++) {
    epsilon[i] = 1;
  }
}

/** Frees the memory. */
DiscretizationPreprocessor::~DiscretizationPreprocessor(void) {
  if (epsilon) {
    free(epsilon);
    epsilon = 0;
  }
}

/** Preprocess a state representation.
    \param S State of the liquid (= filtered response of the neural microcircuit).
    \param X Target vector where to save the results.
    \return -1 if an error occured, 1 for success. */
int DiscretizationPreprocessor::process(const double* S, double* X) {
  if (S == 0) {
    // Error
    TheCsimError.add("DiscretizationPreprocessor::process: Input is a NULL pointer!\n");
    return -1;
  }

  if (X == 0) {
    // Error
    TheCsimError.add("DiscretizationPreprocessor::process: Target is a NULL pointer!\n");
    return -1;
  }


  // Calculate discretization of each input row
  for (int i=0; i<nInputRows; i++) {
    X[i] = (int)( (S[i] / epsilon[i]) +0.5 );
  }

  return 1;
}

/** Resets the information stored within the preprocessor. */
void DiscretizationPreprocessor::reset() {
}

/** Imports the data from an externally (e.g. Matlab) trained preprocessor.
    \param rep Representation of the preprocessor as a double vector.
    Format: first number gives the number of rows for input vectors, the following rows elements are
    in the format [eps_1, eps_2, ..., eps_n], where the transformations are \f$round(x_i / eps_i)\f$.
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int DiscretizationPreprocessor::importRepresentation(const double* rep, int rep_length) {
  if (rep == 0) {
    // Error
    TheCsimError.add("DiscretizationPreprocessor::importRepresentation: Input is a NULL pointer!\n");
    return -1;
  }

  int newRows = (int) rep[0];
  if (newRows < 0) {
    // Error
    TheCsimError.add("DiscretizationPreprocessor::importRepresentation: Invalid number of input rows %i!\n", newRows);
    return -1;
  }

  if ( rep_length != (newRows+1) ) {
    // Error
    TheCsimError.add("DiscretizationPreprocessor::importRepresentation: Length of representation vector is %i but should be %i for %i rows!\n", rep_length, (newRows+1), newRows);
    return -1;
  }

  if (newRows != nInputRows) {
    // Re-Allocate Memory for coefficients
    epsilon = (double *) realloc(epsilon, newRows*sizeof(double));
    nInputRows = nOutputRows = newRows;
    dInputRows = dOutputRows = (double) newRows;
  }

  // Copy the coefficients
  memcpy(epsilon, rep+1, nInputRows*sizeof(double));

  return 1;
}

/** Exports the representation of this preprocessor for use in external objects.
    Format: first number gives the number of rows for input vectors, the following rows elements are
    in the format [eps_1, eps_2, ..., eps_n], where the transformations are \f$round(x_i / eps_i)\f$.
    \param rep_length Length of the representation vector.
    \return A list of parameters that represent the preprocessor.
    \warning Do not forget to free the memory reserved for the representation! */
double* DiscretizationPreprocessor::exportRepresentation(int *rep_length) {
  // Copy the representation into a new memory chunk
  *rep_length = nInputRows+1;
  double *rep = (double *) calloc(nInputRows+1, sizeof(double));
  rep[0] = (double) nInputRows;
  memcpy(rep+1, epsilon, nInputRows * sizeof(double));

  return rep;
}

/** Returns a textual description of the representation format for import/export - Representation. */
string DiscretizationPreprocessor::getFormatDescription() {
  string h = "Discretization Preprocessor: Representation Format:\n";
  h += "Format: first number gives the number of rows for input vectors,\n";
  h += "the following rows elements are in the format [eps_1, eps_2, ..., eps_n],\n";
  h += "where the transformations are round(x_i / eps_i).";
  return h;
}

