/** \file algorithm.cpp
**  \brief Base class of all learning algorithms that calculate the readout's output.
*/

#include "algorithm.h"

/** Constructs a new learning algorithm.
    \param in_rows Number of rows in each input vector. 
    \param lower_bound Lower bound of the algorithms range. 
    \param upper_bound Upper bound of the algorithms range. */
Algorithm::Algorithm(unsigned int in_rows, double lower_bound, double upper_bound) {
  nInputRows = in_rows;
  dInputRows = (double) nInputRows;

  range_low = lower_bound;
  range_high = upper_bound;

  params["input_rows"] = &dInputRows;
  params["range_low"] = &range_low;
  params["range_high"] = &range_high;
}


/** Sets a parameter of the algorithm.
    \param name Name of the parameter.
    \param value Value to set for the parameter. */
void Algorithm::setParameter(string name, double value) {
  map<string, double*>::iterator p;

  // Look if this is a valid parameter
  p = params.find(name);
  if (p != params.end()) {
    // Set the parameter
    double* dest = p->second;
    *dest = value;

    // Update internal state of the filter
    updateInternal();
  }
  else {
    // Error
    TheCsimError.add("Algorithm::setParameter: Unrecognized parameter %s!\n", name.c_str());
  }
}

/** This function is called after parameters are updated. */
int Algorithm::updateInternal() {
  // Copy changed dummy values to the right variables

  /*  
      nInputRows = (int) dInputRows;
      if (nInputRows < 1) {
      nInputRows = 1;
      dInputRows = 1;
      }
      
      return 0;
  */
  return 0;
}



/** Returns the current value of a parameter.
    \param name Name of the parameter.
    \return The value of the parameter. */
double Algorithm::getParameter(string name) {
  map<string, double*>::iterator p;

  // Look if this is a valid parameter
  p = params.find(name);
  if (p != params.end()) {
    // Return parameter value
    return *(p->second);
  }
  else {
    // Error
    TheCsimError.add("Algorithm::setParameter: Unrecognized parameter %s!\n", name.c_str());
  }
}


/** Returns the names of the valid parameters.
    \return A string list indicating the valid parameter names. */
list<string> Algorithm::validParameters(void) {
  list<string> pn;

  map<string, double*>::iterator p = params.begin();

  // Enter the names of all map-keys into the returned list
  while (p != params.end()) {
    pn.push_back(p->first);
    p++;
  }

  return pn;
}


/** Returns the range of the algorithm's target values [a, b].
    \param a Address of lower bound for algorithm's target values.
    \param b Address of upper bound for algorithm's target values. */
void Algorithm::getRange(double* a, double *b) {
  if ((a == 0) || (b == 0)) {
    // Error
    TheCsimError.add("Algorithm::getRange: Cannot store in a NULL pointer!\n");
    return;
  }

  *a = range_low;
  *b = range_high;
}
  
/** Sets the range of the algorithm's target values [a, b].
    \param a Lower bound for algorithm's target values.
    \param b Upper bound for algorithm's target values. */
void Algorithm::setRange(double a, double b) {
  double tmp;
  if (a > b) {
    tmp = a;
    a = b;
    b = tmp;
  }

  range_low = a;
  range_high = b;
}
