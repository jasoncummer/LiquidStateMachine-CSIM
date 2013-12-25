/** \file preprocessor.cpp
**  \brief Base class of all filter objects
*/

#include "preprocessor.h"


/** Constructs a new preprocessor object.
    \param in_rows Number of rows in each input vector.
    \param out_rows Number of rows in each output vector. */
Preprocessor::Preprocessor(unsigned int in_rows, unsigned int out_rows) {
  nInputRows = in_rows;
  dInputRows = (double) nInputRows;
  nOutputRows = out_rows;
  dOutputRows = (double) nOutputRows;

  params["input_rows"] = &dInputRows;
  params["output_rows"] = &dOutputRows;
}


/** Sets a parameter of the preprocessor function.
    \param name Name of the parameter.
    \param value Value to set for the parameter. */
void Preprocessor::setParameter(string name, double value) {
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
    TheCsimError.add("Preprocessor::setParameter: Unrecognized parameter %s!\n", name.c_str());
  }
}

/** This function is called after parameters are updated. */
int Preprocessor::updateInternal() {
  return 0;
}



/** Returns the current value of a parameter.
    \param name Name of the parameter.
    \return The value of the parameter. */
double Preprocessor::getParameter(string name) {
  map<string, double*>::iterator p;

  // Look if this is a valid parameter
  p = params.find(name);
  if (p != params.end()) {
    // Return parameter value
    return *(p->second);
  }
  else {
    // Error
    TheCsimError.add("Preprocessor::setParameter: Unrecognized parameter %s!\n", name.c_str());
  }
}


/** Returns the names of the valid parameters.
    \return A string list indicating the valid parameter names. */
list<string> Preprocessor::validParameters(void) {
  list<string> pn;

  map<string, double*>::iterator p = params.begin();

  // Enter the names of all map-keys into the returned list
  while (p != params.end()) {
    pn.push_back(p->first);
    p++;
  }

  return pn;
}
