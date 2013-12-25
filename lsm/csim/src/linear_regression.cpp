/** \file linear_regression.cpp
**  \brief Implementation of a linear regression.
*/


#include "linear_regression.h"


/** Constructs a new linear regression algorithm.
    \param in_rows Number of rows in each input vector. */
linear_regression::linear_regression(unsigned int in_rows) : Algorithm(in_rows, -1, 1) {
  // Initialize to zero
  regression_coefficients = (double *) calloc(in_rows+1, sizeof(double));
  for (int i=0; i<in_rows+1; i++) {
    regression_coefficients[i] = 0;
  }
}
 

/** Frees the memory. */
linear_regression::~linear_regression(void) {
  if (regression_coefficients)
    free(regression_coefficients);
  regression_coefficients = 0;
}

/** Applies the currently learned function to the filtered and preprocessed input vector.
    \param S State of the liquid (= filtered and preprocessed response of the neural microcircuit).
    \param X Target pointer where to save the result. 
    \return -1 if an error occured, 1 for success. */
int linear_regression::apply(const double* S, double* X) {
  if (S == 0) {
    // Error
    TheCsimError.add("linear_regression::apply: Input is a NULL pointer!\n");
    return -1;
  }

  if (X == 0) {
    // Error
    TheCsimError.add("linear_regression::apply: Target is a NULL pointer!\n");
    return -1;
  }


  // Calculate weighted sum of input vector

  double result = 0;
  int i;

  for (i=0; i<nInputRows; i++)
    result += S[i] * regression_coefficients[i];

  result += regression_coefficients[nInputRows];

  *X = result;
  
  return 1;
}

/** Resets the information stored within the algorithm. */
void linear_regression::reset() {
}

/** Imports the data from an externally (e.g. Matlab) trained algorithm.
    Format: first number gives the number of rows for input vectors, the following rows+1 elements are
    in the format [w_1, ..., w_n, w_0], where the algorithm calculates \f$w_0 + x_i \cdot w_i\f$.
    \param rep Representation of the algorithm as a double vector.
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int linear_regression::importRepresentation(const double* rep, int rep_length) {
  if (rep == 0) {
    // Error
    TheCsimError.add("linear_regression::importRepresentation: Input is a NULL pointer!\n");
    return -1;
  }

  int newRows = (int) rep[0];
  if (newRows < 0) {
    // Error
    TheCsimError.add("linear_regression::importRepresentation: Invalid number of input rows %i!\n", newRows);
    return -1;
  }

  if ( rep_length != (newRows+2) ) {
    // Error
    TheCsimError.add("linear_regression::importRepresentation: Length of representation vector is %i but should be %i for %i rows!\n", rep_length, (newRows+2), newRows);
    return -1;
  }

  if (newRows != nInputRows) {
    // Re-Allocate Memory for coefficients
    regression_coefficients = (double *) realloc(regression_coefficients, (newRows+1)*sizeof(double));
    nInputRows = newRows;
    dInputRows = (double) newRows;
  }

  // Copy the coefficients
  memcpy(regression_coefficients, rep+1, (nInputRows+1)*sizeof(double));

  return 1;
}

/** Exports the representation of this algorithm for use in external objects.
    Format: first number gives the number of rows for input vectors, the following rows+1 elements are
    in the format [w_1, ..., w_n, w_0], where the algorithm calculates \f$w_0 + \sum_0^n x_i \cdot w_i\f$.
    \return A list of parameters that represent the algorithm.
    \warning Do not forget to free the memory reserved for the representation! */
double* linear_regression::exportRepresentation(int *rep_length) {
  // Copy the representation into a new memory chunk
  *rep_length = nInputRows+2;
  double *rep = (double *) calloc(nInputRows+2, sizeof(double));
  rep[0] = (double) nInputRows;
  memcpy(rep+1, regression_coefficients, (nInputRows+1)*sizeof(double));

  return rep;
}


/** Returns a textual description of the representation format for import/export - Representation. */
string linear_regression::getFormatDescription() {
  string h = "Linear Regression: Representation Format:\n";
  h += "First number gives the number of rows for input vectors, \n";
  h += "the following rows+1 elements are in the format [w_1, ..., w_n, w_0],\n";
  h += "where the algorithm calculates w_0 + sum_0^n x_i * w_i.";
  return h;
}

/*
#include <iostream>

int main() {
  linear_regression *lr = new linear_regression(5);

  double rep[7] = {5, -3, 1, 1, 1, 1, 1};

  lr->importRepresentation(rep);

  double X[5] = {1, 2, 3, 4, 5};

  double result;

  lr->apply(X, &result);

  cout << "Result: " << result << "\n";

  double *exp = lr->exportRepresentation();

  for (int i=0; i<7; i++)
    cout << exp[i] << ", ";

  cout << "\n";

  delete lr;
}
*/
