/** \file mean_std_preprocessor.cpp
**  \brief Implementation of a Mean / Standard-Deviation Normalizer.
*/

#include "mean_std_preprocessor.h" 

/** Constructs a new normalization transformation. Initially the transformation is an identical transformation x'=x. 
    \param rows Number of rows in each input vector. */
Mean_Std_Preprocessor::Mean_Std_Preprocessor(unsigned int rows) : Preprocessor(rows, rows) {

  // Initialize to identical transformation
  means = (double *) calloc(rows, sizeof(double));
  std_devs = (double *) calloc(rows, sizeof(double));
  for (int i=0; i<rows; i++) {
    means[i] = 0;
    std_devs[i] = 1;
  }
}

/** Frees the memory. */
Mean_Std_Preprocessor::~Mean_Std_Preprocessor(void) {
  if (means)
    free(means);
  means = 0;
  if (std_devs)
    free(std_devs);
  std_devs = 0;
}

/** Preprocess a state representation.
    \param S State of the liquid (= filtered response of the neural microcircuit).
    \param X Target vector where to save the results. 
    \return -1 if an error occured, 1 for success. */
int Mean_Std_Preprocessor::process(const double* S, double* X) {
  if (S == 0) {
    // Error
    TheCsimError.add("Mean_Std_Preprocessor::process: Input is a NULL pointer!\n");
    return -1;
  }

  if (X == 0) {
    // Error
    TheCsimError.add("Mean_Std_Preprocessor::process: Target is a NULL pointer!\n");
    return -1;
  }



  // Calculate normalization of the input

  for (int i=0; i<nInputRows; i++) {
    X[i] = (S[i] - means[i]) / std_devs[i];
  }
  
  return 1;
}

/** Resets the information stored within the preprocessor. */
void Mean_Std_Preprocessor::reset() {
}

/** Imports the data from an externally (e.g. Matlab) trained preprocessor.
    \param rep Representation of the preprocessor as a double vector. 
    Format: first number gives the number of rows for input vectors, the following 2*rows elements are
    in the format [m_1, m_2, ..., m_n, s_1, s_2, ..., s_n], where the transformations are \f$(x_i - m_i) / s_i\f$.
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int Mean_Std_Preprocessor::importRepresentation(const double* rep, int rep_length) {
  if (rep == 0) {
    // Error
    TheCsimError.add("Mean_Std_Preprocessor::importRepresentation: Input is a NULL pointer!\n");
    return -1;
  }

  int newRows = (int) rep[0];
  if (newRows < 0) {
    // Error
    TheCsimError.add("Mean_Std_Preprocessor::importRepresentation: Invalid number of input rows %i!\n", newRows);
    return -1;
  }

  if ( rep_length != (2*newRows+1) ) {
    // Error
    TheCsimError.add("Mean_Std_Preprocessor::importRepresentation: Length of representation vector is %i but should be %i for %i rows!\n", rep_length, (2*newRows+1), newRows);
    return -1;
  }


  if (newRows != nInputRows) {
    // Re-Allocate Memory for coefficients
    means = (double *) realloc(means, newRows*sizeof(double));
    std_devs = (double *) realloc(std_devs, newRows*sizeof(double));
    nInputRows = nOutputRows = newRows;
    dInputRows = dOutputRows = (double) newRows;
  }

  // Copy the coefficients
  for (int i=0; i<nInputRows; i++) {
    means[i] = rep[i+1];
    std_devs[i] = rep[i+nInputRows+1];
  }
  return 1;
}

/** Exports the representation of this preprocessor for use in external objects.
    Format: first number gives the number of rows for input vectors, the following 2*rows elements are
    in the format [m_1, m_2, ..., m_n, s_1, s_2, ..., s_n], where the transformations are \f$(x_i - m_i) / s_i\f$.
    \param rep_length Length of the representation vector.
    \return A list of parameters that represent the preprocessor. 
    \warning Do not forget to free the memory reserved for the representation! */
double* Mean_Std_Preprocessor::exportRepresentation(int *rep_length) {
  // Copy the representation into a new memory chunk
  *rep_length = 2*nInputRows+1;
  double *rep = (double *) calloc(2*nInputRows+1, sizeof(double));
  rep[0] = (double) nInputRows;

  for (int i=0; i<nInputRows; i++) {
    rep[i+1] = means[i];
    rep[i+nInputRows+1] = std_devs[i];
  }

  return rep;
}

/** Returns a textual description of the representation format for import/export - Representation. */
string Mean_Std_Preprocessor::getFormatDescription() {
  string h = "Normalization Preprocessor: Representation Format:\n";
  h += "Format: first number gives the number of rows for input vectors,\n";
  h += "the following 2*rows elements are in the format [m_1, m_2, ..., m_n, s_1, s_2, ..., s_n],\n";
  h += "where the transformations are (x_i - m_i) / s_i.";
  return h;
}

