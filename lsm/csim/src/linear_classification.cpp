/** \file linear_classification.cpp
**  \brief Implementation of a linear classification.
*/


#include "linear_classification.h"


/** Constructs a new linear classification algorithm.
    \param in_rows Number of rows in each input vector. */
linear_classification::linear_classification(unsigned int in_rows, unsigned int classes) : Algorithm(in_rows, 0, 1) {
  // Initialize to zero
  if (classes == 2) {
    // 2-class problems are handled differently
    regression_coefficients = (double **) calloc(1, sizeof(double *));
    *regression_coefficients = (double *) calloc(in_rows+1, sizeof(double));
    for (int i=0; i<in_rows+1; i++) {
      regression_coefficients[0][i] = 0;
    }
  }
  else {
    // Multi-class problem
    regression_coefficients = (double **) calloc(classes, sizeof(double *));
    for (int i=0; i<classes; i++) {
      regression_coefficients[i] = (double *) calloc(in_rows+1, sizeof(double));
      for (int j=0; j<in_rows+1; j++) {
	regression_coefficients[i][j] = 0;
      }
    }
  }

  addBias = 1;
  nClasses = classes;
}
 

/** Frees the memory. */
linear_classification::~linear_classification(void) {
  if (regression_coefficients) {
    if (nClasses == 2) {
      if (*regression_coefficients)
	free(*regression_coefficients);
      free(regression_coefficients);
    }
    else {
      for (int i=0; i<nClasses; i++) {
	if (regression_coefficients[i])
	  free(regression_coefficients[i]);
      }
      free(regression_coefficients);
    }
  }
  regression_coefficients = 0;
}


double linear_classification::weighted_sum(const double* S, const double* w) {
  int bias = 0;
  double result = 0;
  if (addBias) {
    bias = 1;
    result = w[nInputRows];
  }
  for (int i=0; i<nInputRows; i++) {
    result += S[i] * w[i];
  }
  return result;
}


/** Applies the currently learned function to the filtered and preprocessed input vector.
    \param S State of the liquid (= filtered and preprocessed response of the neural microcircuit).
    \param X Target pointer where to save the result. 
    \return -1 if an error occured, 1 for success. */
int linear_classification::apply(const double* S, double* X) {
  if (S == 0) {
    // Error
    TheCsimError.add("linear_classification::apply: Input is a NULL pointer!\n");
    return -1;
  }

  if (X == 0) {
    // Error
    TheCsimError.add("linear_classification::apply: Target is a NULL pointer!\n");
    return -1;
  }

  if (nClasses == 2) {
    // binary classification
    double res = weighted_sum(S, regression_coefficients[0]);
    *X = (double) (res >= 0);
    return 1;
  }
  else {
    // multi-class classification
    double max_v = 0;
    int max_ind = -1;
    double res;

    // Calculate class with highest weighted sum
    for (int i=0; i<nClasses; i++) {
      csimPrintf("%d: ", i);
      res = weighted_sum(S, regression_coefficients[i]);
      if ((res > max_v) || (max_ind < 0)) {
	max_v = res;
	max_ind = i;
      }
    }

    *X = (double) max_ind;
    return 1;
  }
}

/** Resets the information stored within the algorithm. */
void linear_classification::reset() {
}

/** Imports the data from an externally (e.g. Matlab) trained algorithm.
    Format: 0: the number of rows for input vectors, 
	    1: add bias or not (0 == no, 1 == yes), if no: w_0 is set to 0 (it must not be included in parameter list!),
            2: number of classes,
	    if number of classes is 2, then the following rows+1 elements are
	    in the format [w_1, ..., w_n, w_0], where the algorithm calculates \f$w_0 + sum_i  x_i \cdot w_i >= 0\f$.
	    if more than 2 classes are used, then the following (classes * (rows+1)) elements are in the format
	    [w_11, ..., w_1n, w_10, w_21, ..., w_2n, w_20..., w_m1, ..., w_mn, w_m0], where the algorithm calculates
	    \f$w_i0 + sum_j x_j \cdot w_ij\f$ and outputs the class i with the highest weighted sum.
    \param rep Representation of the algorithm as a double vector.
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int linear_classification::importRepresentation(const double* rep, int rep_length) {
  if (rep == 0) {
    // Error
    TheCsimError.add("linear_classification::importRepresentation: Input is a NULL pointer!\n");
    return -1;
  }

  int newRows = (int) rep[0];
  if (newRows < 0) {
    // Error
    TheCsimError.add("linear_classification::importRepresentation: Invalid number of input rows %i!\n", newRows);
    return -1;
  }

  int newBias = (int) rep[1];
  if ((newBias < 0) || (newBias > 1)) {
    // Error
    TheCsimError.add("linear_classification::importRepresentation: Invalid Bias flag %i!\n", newBias);
    return -1;
  }

  int newClasses = (int) rep[2];
  if (newClasses < 2) {
    // Error
    TheCsimError.add("linear_classification::importRepresentation: Invalid number of classes %i! Need at least 2!\n", newClasses);
    return -1;
  }

  int needed_length = 3;
  if (newClasses == 2) {
    needed_length += newRows + newBias;
  }
  else {
    needed_length += newClasses * (newRows + newBias);
  }

  if ( rep_length != needed_length ) {
    // Error
    TheCsimError.add("linear_classification::importRepresentation: Length of representation vector is %i but should be %i!\n", rep_length, needed_length);
    return -1;
  }

  if (newClasses == 2) {
    // Import binary classificator

    if ((newRows != nInputRows) || (newClasses != nClasses) || ((newBias > 0) != (addBias))) {
      // Re-Allocate Memory for coefficients
      regression_coefficients = (double **) realloc(regression_coefficients, sizeof(double *));
      regression_coefficients[0] = (double *) realloc(regression_coefficients[0], (newRows+newBias)*sizeof(double));
      nInputRows = newRows;
      dInputRows = (double) newRows;
      nClasses = newClasses;
      addBias = (newBias > 0);
    }

    // Copy the coefficients
    memcpy(*regression_coefficients, rep+3, (needed_length-3)*sizeof(double));
    return 1;
  }
  else {
    // Import multi-class classificator
    if ((newRows != nInputRows) || (newClasses != nClasses) || ((newBias > 0) != (addBias))) {
      // Re-Allocate Memory for coefficients
      regression_coefficients = (double **) realloc(regression_coefficients, newClasses*sizeof(double *));
      for (int i=0; i<newClasses; i++)
	regression_coefficients[i] = (double *) realloc(regression_coefficients[i], (newRows+newBias)*sizeof(double));
      nInputRows = newRows;
      dInputRows = (double) newRows;
      nClasses = newClasses;
      addBias = (newBias > 0);
    }
     
    // Copy the coefficients
    const double *source = rep+3;
    for (int i=0; i<newClasses; i++) {
      memcpy(regression_coefficients[i], source, (newRows+newBias)*sizeof(double));
      source += (newRows + newBias);
    }
    return 1;
  }
}

/** Exports the representation of this algorithm for use in external objects.
    Format: 0: the number of rows for input vectors, 
	    1: add bias or not (0 == no, 1 == yes), if no: w_0 is set to 0 (it must not be included in parameter list!),
            2: number of classes,
	    if number of classes is 2, then the following rows+1 elements are
	    in the format [w_0, w_1, ..., w_n], where the algorithm calculates \f$w_0 + sum_i  x_i \cdot w_i >= 0\f$.
	    if more than 2 classes are used, then the following (classes * (rows+1)) elements are in the format
	    [w_11, ..., w_1n, w_10, w_21, ..., w_2n, w_20..., w_m1, ..., w_mn, w_m0], where the algorithm calculates
	    \f$w_i0 + sum_j x_j \cdot w_ij\f$ and outputs the class i with the highest weighted sum.
    \return A list of parameters that represent the algorithm.
    \warning Do not forget to free the memory reserved for the representation! */
double* linear_classification::exportRepresentation(int *rep_length) {
  // Copy the representation into a new memory chunk
  if (nClasses == 2) {
    // export binary classificator
    int len = 3 + nInputRows + addBias;
    *rep_length = len;

    double *rep = (double *) calloc(len, sizeof(double));
    rep[0] = (double) nInputRows;
    rep[1] = (double) addBias;
    rep[2] = (double) nClasses;
    memcpy(rep+3, regression_coefficients, (len-3)*sizeof(double));
    return rep;
  }
  else {
    // export multi-class classificator
    int linelen = nInputRows + addBias;
    int len = 3 + nClasses * linelen;
    *rep_length = len;

    double *rep = (double *) calloc(len, sizeof(double));
    rep[0] = (double) nInputRows;
    rep[1] = (double) addBias;
    rep[2] = (double) nClasses;

    double *dst = rep+3;
    for (int i=0; i<nClasses; i++) {
      memcpy(dst, regression_coefficients[i], linelen*sizeof(double));
      dst += linelen;
    }
    return rep;
  }
}


/** Returns a textual description of the representation format for import/export - Representation. */
string linear_classification::getFormatDescription() {
  string h = "Linear Classification: Representation Format:\n";
  h += "0: the number of rows for input vectors,\n"; 
  h += "1: add bias or not (0 == no, 1 == yes), if no: w_0 is set to 0 (it must not be included in parameter list!),\n";
  h += "2: number of classes,\n";
  h += "if number of classes is 2, then the following rows+1 elements are\n";
  h += "in the format [w_0, w_1, ..., w_n], where the algorithm calculates w_0 + sum_i  x_i * w_i >= 0.\n";
  h += "if more than 2 classes are used, then the following (classes * (rows+1)) elements are in the format\n";
  h += "[w_11, ..., w_1n, w_10, w_21, ..., w_2n, w_20..., w_m1, ..., w_mn, w_m0], where the algorithm calculates\n";
  h += "w_i0 + sum_j x_j * w_ij and outputs the class i with the highest weighted sum.\n";
  return h;
}
