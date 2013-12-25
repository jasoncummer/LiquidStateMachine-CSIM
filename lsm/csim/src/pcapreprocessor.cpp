/** \file pcapreprocessor.cpp
**  \brief Implementation of a PCA (Principal Component Analysis) of the input.
*/

#include "pcapreprocessor.h"

/** Constructs a PCA transformation. Initially the transformation is an identical transformation x'=x. 
    \param in_rows Number of rows in each input vector.
    \param out_rows Number of rows in each output vector. */
PCAPreprocessor::PCAPreprocessor(unsigned int in_rows, unsigned int out_rows) : Preprocessor(in_rows, out_rows) {

  // Initialize to identical transformation
  int num_elements = in_rows * out_rows;

  pca_matrix = (double *) calloc(num_elements, sizeof(double));
  int pos = 0;
  for (int i=0; i<in_rows; i++)
    for (int j=0; j<out_rows; j++) {
      if (i==j)
	pca_matrix[pos++] = 1;
      else
	pca_matrix[pos++] = 0;
    }
}

/** Frees the memory. */
PCAPreprocessor::~PCAPreprocessor(void) {
  if (pca_matrix) {
    free(pca_matrix);
    pca_matrix = 0;
  }
}

/** Preprocess a state representation.
    \param S State of the liquid (= filtered response of the neural microcircuit).
    \param X Target vector where to save the results. 
    \return -1 if an error occured, 1 for success. */
int PCAPreprocessor::process(const double* S, double* X) {
  if (S == 0) {
    // Error
    TheCsimError.add("PCAPreprocessor::process: Input is a NULL pointer!\n");
    return -1;
  }

  if (X == 0) {
    // Error
    TheCsimError.add("PCAPreprocessor::process: Target is a NULL pointer!\n");
    return -1;
  }


  // Calculate PCA rotation of each row
  int i, j;
  double tmp;
  double *p = pca_matrix;

  for (i=0; i<nOutputRows; i++) {
    tmp = 0;
    for (j=0; j<nInputRows; j++) {
      tmp += S[j] * (*p);
      p++;
    }
    X[i] = tmp;
  }

  return 1;
}

/** Resets the information stored within the preprocessor. */
void PCAPreprocessor::reset() {
}

/** Imports the data from an externally (e.g. Matlab) trained preprocessor.
    \param rep Representation of the preprocessor as a double vector. 
    Format: first number gives the number of rows for input vectors, second number gives the number of rows output vectors,
    the following rows elements are the elements of the (m x n) PCA transformation matrix (row by row). The format is 
    [a_11, a_12, ..., a_1n,a_21, ..., a_m1, ..., a_mn].
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int PCAPreprocessor::importRepresentation(const double* rep, int rep_length) {
  if (rep == 0) {
    // Error
    TheCsimError.add("PCAPreprocessor::importRepresentation: Input is a NULL pointer!\n");
    return -1;
  }

  int newInRows = (int) rep[0];
  if (newInRows < 0) {
    // Error
    TheCsimError.add("PCAPreprocessor::importRepresentation: Invalid number of input rows %i!\n", newInRows);
    return -1;
  }

  int newOutRows = (int) rep[1];
  if (newOutRows < 0) {
    // Error
    TheCsimError.add("PCAPreprocessor::importRepresentation: Invalid number of output rows %i!\n", newOutRows);
    return -1;
  }

  if ( rep_length != (newInRows*newOutRows+2) ) {
    // Error
    TheCsimError.add("PCAPreprocessor::importRepresentation: Length of representation vector is %i but should be %i for %i input- and %i output-rows!\n", rep_length, (newInRows*newOutRows+2), newInRows, newOutRows);
    return -1;
  }



  if ((newInRows != nInputRows) || (newOutRows != nOutputRows)) {
    // Re-Allocate Memory for coefficients
    pca_matrix = (double *) realloc(pca_matrix, newInRows*newOutRows*sizeof(double));
    nInputRows = newInRows; dInputRows = (double) nInputRows;
    nOutputRows = newOutRows; dOutputRows = (double) nOutputRows;
  }

  // Copy the coefficients
  memcpy(pca_matrix, rep+2, nInputRows*nOutputRows*sizeof(double));

  return 1;
}

/** Exports the representation of this preprocessor for use in external objects.
    Format: first number gives the number of rows for input vectors, second number gives the number of rows output vectors,
    the following rows elements are the elements of the (m x n) PCA transformation matrix (row by row). The format is 
    [a_11, a_12, ..., a_1n,a_21, ..., a_m1, ..., a_mn].
    \param rep_length Length of the representation vector.
    \return A list of parameters that represent the preprocessor. 
    \warning Do not forget to free the memory reserved for the representation! */
double* PCAPreprocessor::exportRepresentation(int *rep_length) {
  // Copy the representation into a new memory chunk
  *rep_length = nInputRows*nOutputRows + 2;
  double *rep = (double *) calloc(nInputRows*nOutputRows + 2, sizeof(double));
  rep[0] = (double) nInputRows;
  rep[1] = (double) nOutputRows;
  memcpy(rep+2, pca_matrix, nInputRows * nOutputRows * sizeof(double));

  return rep;
}

/** Returns a textual description of the representation format for import/export - Representation. */
string PCAPreprocessor::getFormatDescription() {
  string h = "PCA Preprocessor: Representation Format:\n";
  h += "First number gives the number of rows for input vectors,\n";
  h += "second number gives the number of rows output vectors,\n";
  h += "the following rows elements are the elements of the (m x n)\n";
  h += "PCA transformation matrix (row by row). The format is \n";
  h += "[a_11, a_12, ..., a_1n,a_21, ..., a_m1, ..., a_mn].";
  return h;
}

