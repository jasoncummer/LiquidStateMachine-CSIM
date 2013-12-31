/** \file mean_std_preprocessor.h
**  \brief Implementation of a Mean / Standard-Deviation Normalizer.
*/

#ifndef _MEAN_STD_PREPROCESSOR_H_
#define _MEAN_STD_PREPROCESSOR_H_

#include <string>
#include <map>
#include <list>
#include "preprocessor.h"
#include "csimerror.h"

using namespace std;

//! Implementation of a Mean / Standard-Deviation Normalizer.
/** Every row x_i of the input vector is transformed into \f$x_i' = (x_i - m_i) / std_i\f$, where m_i and std_i are the mean and standard deviation of the i-th row. */
class Mean_Std_Preprocessor : public Preprocessor {

  DO_REGISTERING
  
 public:
  /** Constructs a new normalization transformation. Initially the transformation is an identical transformation x'=x.
      \param rows Number of rows in each input vector. */
  Mean_Std_Preprocessor(unsigned int rows = 1);

  ~Mean_Std_Preprocessor(void);

  /** Preprocess a state representation.
      \param S State of the liquid (= filtered response of the neural microcircuit).
      \param X Target vector where to save the results. 
      \return -1 if an error occured, 1 for success. */
  int process(const double* S, double* X);

  /** Resets the information stored within the preprocessor. */
  void reset();

  /** Imports the data from an externally (e.g. Matlab) trained preprocessor.
      \param rep Representation of the preprocessor as a double vector. 
      Format: first number gives the number of rows for input vectors, the following 2*rows elements are
      in the format [m_1, m_2, ..., m_n, s_1, s_2, ..., s_n], where the transformations are \f$(x_i - m_i) / s_i\f$.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  int importRepresentation(const double* rep, int rep_length);

  /** Exports the representation of this preprocessor for use in external objects.
      Format: first number gives the number of rows for input vectors, the following 2*rows elements are
      in the format [m_1, s_1, m_2, s_2, ..., m_n, s_n], where the transformations are \f$(x_i - m_i) / s_i\f$.
      \param rep_length Length of the representation vector.
      \return A list of parameters that represent the preprocessor. 
      \warning Do not forget to free the memory reserved for the representation! */
  double* exportRepresentation(int *rep_length);

  /** Returns a textual description of the representation format for import/export - Representation. */
  string getFormatDescription();

 private:

  //! The means of each input row.
  double *means;

  //! The standard deviations of each input row.
  double *std_devs;

  
};

#endif
