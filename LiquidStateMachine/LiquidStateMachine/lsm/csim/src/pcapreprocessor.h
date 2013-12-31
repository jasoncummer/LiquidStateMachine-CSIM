/** \file pcapreprocessor.h
**  \brief Implementation of a PCA (Principal Component Analysis) of the input.
*/

#ifndef _PCAPREPROCESSOR_H_
#define _PCAPREPROCESSOR_H_

#include <string>
#include <map>
#include <list>
#include "preprocessor.h"
#include "csimerror.h"

using namespace std;

//!  Implementation of a PCA (Principal Component Analysis) of the input. A principal component transformation is applied to the whole input vector.
class PCAPreprocessor : public Preprocessor{

  DO_REGISTERING

 public:
  /** Constructs a PCA transformation. Initially the transformation is an identical transformation x'=x. 
      \param in_rows Number of rows in each input vector.
      \param out_rows Number of rows in each output vector. */
  PCAPreprocessor(unsigned int in_rows = 1, unsigned int out_rows = 1);

  ~PCAPreprocessor(void);

  /** Preprocess a state representation.
      \param S State of the liquid (= filtered response of the neural microcircuit).
      \param X Target vector where to save the results. 
      \return -1 if an error occured, 1 for success. */
  int process(const double* S, double* X);

  /** Resets the information stored within the preprocessor. */
  void reset();

  /** Imports the data from an externally (e.g. Matlab) trained preprocessor.
      \param rep Representation of the preprocessor as a double vector. 
      Format: first number gives the number of rows for input vectors, second number gives the number of rows output vectors,
      the following rows elements are the elements of the (m x n) PCA transformation matrix (row by row). The format is 
      [a_11, a_12, ..., a_1n,a_21, ..., a_m1, ..., a_mn].
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  int importRepresentation(const double* rep, int rep_length);

  /** Exports the representation of this preprocessor for use in external objects.
      Format: first number gives the number of rows for input vectors, second number gives the number of rows output vectors,
      the following rows elements are the elements of the (m x n) PCA transformation matrix (row by row). The format is 
      [a_11, a_12, ..., a_1n,a_21, ..., a_m1, ..., a_mn].
      \param rep_length Length of the representation vector.
      \return A list of parameters that represent the preprocessor. 
      \warning Do not forget to free the memory reserved for the representation! */
  double* exportRepresentation(int *rep_length);

  /** Returns a textual description of the representation format for import/export - Representation. */
  string getFormatDescription();


 private:

  //! The PCA transformation matrix.
  double *pca_matrix;
  
};

#endif
