/** \file linearpreprocessor.cpp
**  \brief Implementation of a linear transformation of the input.
*/

#include "linearpreprocessor.h"

/** Constructs a new linear transformation. Initially the transformation is an identical transformation x'=x. 
    \param rows Number of rows in each input vector. */
LinearPreprocessor::LinearPreprocessor(unsigned int rows) : Preprocessor(rows, rows) {

  // Initialize to identical transformation
  linear_coefficients = (double *) calloc(2*rows, sizeof(double));
  for (int i=0; i<rows; i++) {
    linear_coefficients[2*i] = 1;
    linear_coefficients[2*i+1] = 0;
  }
}

/** Frees the memory. */
LinearPreprocessor::~LinearPreprocessor(void) {
  if (linear_coefficients) {
    free(linear_coefficients);
    linear_coefficients = 0;
  }
}

/** Preprocess a state representation.
    \param S State of the liquid (= filtered response of the neural microcircuit).
    \param X Target vector where to save the results. 
    \return -1 if an error occured, 1 for success. */
int LinearPreprocessor::process(const double* S, double* X) {
  if (S == 0) {
    // Error
    TheCsimError.add("LinearPreprocessor::process: Input is a NULL pointer!\n");
    return -1;
  }

  if (X == 0) {
    // Error
    TheCsimError.add("LinearPreprocessor::process: Target is a NULL pointer!\n");
    return -1;
  }


  // Calculate linear transformation of each input row
  int pos = 0;

  for (int i=0; i<nInputRows; i++) {
    X[i] = S[i] * linear_coefficients[pos] + linear_coefficients[pos+1];
    pos += 2;
  }

  return 1;
}

/** Resets the information stored within the preprocessor. */
void LinearPreprocessor::reset() {
}

/** Imports the data from an externally (e.g. Matlab) trained preprocessor.
    \param rep Representation of the preprocessor as a double vector. 
    Format: first number gives the number of rows for input vectors, the following 2*rows elements are
    in the format [a_1, b_1, a_2, b_2, ..., a_n, b_n], where the transformations are \f$x_i \cdot a_i + b_i\f$.
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int LinearPreprocessor::importRepresentation(const double* rep, int rep_length) {
  if (rep == 0) {
    // Error
    TheCsimError.add("LinearPreprocessor::importRepresentation: Input is a NULL pointer!\n");
    return -1;
  }

  int newRows = (int) rep[0];
  if (newRows < 0) {
    // Error
    TheCsimError.add("LinearPreprocessor::importRepresentation: Invalid number of input rows %i!\n", newRows);
    return -1;
  }

  if ( rep_length != (2*newRows+1) ) {
    // Error
    TheCsimError.add("LinearPreprocessor::importRepresentation: Length of representation vector is %i but should be %i for %i rows!\n", rep_length, (2*newRows+1), newRows);
    return -1;
  }



  if (newRows != nInputRows) {
    // Re-Allocate Memory for coefficients
    linear_coefficients = (double *) realloc(linear_coefficients, 2*newRows*sizeof(double));
    nInputRows = nOutputRows = newRows;
    dInputRows = dOutputRows = (double) newRows;
  }

  // Copy the coefficients
  memcpy(linear_coefficients, rep+1, 2*nInputRows*sizeof(double));

  return 1;
}

/** Exports the representation of this preprocessor for use in external objects.
    Format: first number gives the number of rows for input vectors, the following 2*rows elements are
    in the format [a_1, b_1, a_2, b_2, ..., a_n, b_n], where the transformations are \f$x_i \cdot a_i + b_i\f$.
    \param rep_length Length of the representation vector.
    \return A list of parameters that represent the preprocessor. 
    \warning Do not forget to free the memory reserved for the representation! */
double* LinearPreprocessor::exportRepresentation(int *rep_length) {
  // Copy the representation into a new memory chunk
  *rep_length = 2*nInputRows+1;
  double *rep = (double *) calloc(2*nInputRows+1, sizeof(double));
  rep[0] = (double) nInputRows;
  memcpy(rep+1, linear_coefficients, 2*nInputRows*sizeof(double));

  return rep;
}


/** Returns a textual description of the representation format for import/export - Representation. */
string LinearPreprocessor::getFormatDescription() {
  string h = "Linear Preprocessor: Representation Format:\n";
  h += "Format: first number gives the number of rows for input vectors,\n";
  h += "the following 2*rows elements are in the format [a_1, b_1, a_2, b_2, ..., a_n, b_n],\n";
  h += "where the transformations are x_i * a_i + b_i.";
  return h;
}





