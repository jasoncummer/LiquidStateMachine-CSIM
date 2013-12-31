/** \file filterfunction.cpp
**  \brief Base class of all filter objects
*/

#include "filterfunction.h"


/** Initializes a new filter. */
FilterFunction::FilterFunction(void) {
}


/** Sets a parameter of the filter function.
    \param name Name of the parameter.
    \param value Value to set for the parameter. */
void FilterFunction::setParameter(string name, double value) {
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
    TheCsimError.add("FilterFunction::setParameter: Unrecognized parameter %s!\n", name.c_str());
  }
}


/** Returns the current value of a parameter.
    \param name Name of the parameter.
    \return The value of the parameter. */
double FilterFunction::getParameter(string name) {
  map<string, double*>::iterator p;

  // Look if this is a valid parameter
  p = params.find(name);
  if (p != params.end()) {
    // Return parameter value
    return *(p->second);
  }
  else {
    // Error
    TheCsimError.add("FilterFunction::setParameter: Unrecognized parameter %s!\n", name.c_str());
  }
}


/** Returns the names of the valid parameters.
    \return A string list indicating the valid parameter names. */
list<string> FilterFunction::validParameters(void) {
  list<string> pn;

  map<string, double*>::iterator p = params.begin();

  // Enter the names of all map-keys into the returned list
  while (p != params.end()) {
    pn.push_back(p->first);
    p++;
  }

  return pn;
}













