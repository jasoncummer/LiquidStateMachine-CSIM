/** \file physicalmodel.cpp
**  \brief Base class of all phyiscal models for CSIM simulations.
*/

#include "physicalmodel.h"

#include <iostream>

using namespace std ;

#define OUTPUTARRAY_INITIAL 10
#define OUTPUTARRAY_INC     10

/** Constructs a new physical model. */
PhysicalModel::PhysicalModel(int input_channels = 0, int output_channels = 0) {

  // Reserve space for inputs
  nIncoming      = input_channels;
  incoming       = (Advancable **)malloc(nIncoming*sizeof(Advancable *));
  int i;
  for (i=0; i<nIncoming; i++)
    incoming[i] = 0;

  inputs = (double **) malloc(nIncoming*sizeof(double *));

  // Reserve space for outputs
  nOutputChannels = output_channels;
  nOutgoing      = (int *) malloc(nOutputChannels*sizeof(int));
  nOutgoingAlloc = (int *) malloc(nOutputChannels*sizeof(int));
  outgoing = (AnalogSynapse ***) malloc(nOutputChannels*sizeof(AnalogSynapse **));
  for (i=0; i<nOutputChannels; i++) {
    nOutgoing[i] = 0;
    nOutgoingAlloc[i] = OUTPUTARRAY_INITIAL;
    outgoing[i] = (AnalogSynapse **) malloc(nOutgoingAlloc[i] * sizeof(AnalogSynapse *));
  }

  outputs = (double *) malloc(nOutputChannels*sizeof(double));

  last_input_index = 0;
  last_output_index = 0;
}

/** Frees the memory */
PhysicalModel::~PhysicalModel() {
  if (incoming)
    free(incoming);
  incoming = 0;

  if (outgoing) {
    for (int i=0; i<nOutputChannels; i++) {
      free(outgoing[i]);
      outgoing[i] = 0;
    }
    free(outgoing);
    outgoing = 0;
  }

  if (nOutgoing)
    free(nOutgoing);
  nOutgoing = 0;
  if (nOutgoingAlloc)
    free(nOutgoingAlloc);
  nOutgoingAlloc = 0;

  if (inputs)
    free(inputs);
  inputs = 0;
  if (outputs)
    free(outputs);
  outputs = 0;
}

//! Calculates the next output of the physical model.
int PhysicalModel::advance(void) {
	 int i, j;
  double out_tmp;

  // Get input data
  // Data is stored in inputs array

  transform(inputs, outputs);

  // Write data to outputs
  for (i=0; i < nOutputChannels; i++) {
    out_tmp = outputs[i];
    for (j=0; j<nOutgoing[i]; j++) {
      outgoing[i][j]->psi = out_tmp;
    }
  }
  return 0;
}

//! Connect a readout output to a named input of the model
//! \param inputname Name of the input channel
int PhysicalModel::addInput(Advancable *Incoming, char *inputname) {
  ModelInput *inp = dynamic_cast<ModelInput *>(Incoming);
  if ( inp ) {
    // Find index corresponding to input-channel name
    string *name = new string(inputname);
    map<string, int>::iterator p;
    p = input_channel_names.find(*name);
    delete name;

    if (p != input_channel_names.end()) {
      // Insert input channel
      incoming[p->second] = Incoming;
      inputs[p->second] = &(inp->output);

      return 0;
    }
    else {
      TheCsimError.add("PhysicalModel::addInput Invalid input-channel name %s!\n",inputname);
      return -1;
    }
  } else {
    TheCsimError.add("PhysicalModel::addInput can not use %s %i as incoming element!\n",Incoming->className(),Incoming->getId());
    return -1;
  }
}

//! Connect an output channel of the model to a synapse
//! \param outputname Name of the output channel
int PhysicalModel::addOutput(Advancable *Outgoing, char *outputname) {
  AnalogSynapse *syn;
  if ( (syn=dynamic_cast<AnalogSynapse *>(Outgoing)) ) {
    // Find index corresponding to output-channel name
    string *name = new string(outputname);
    map<string, int>::iterator p;
    p = output_channel_names.find(*name);
    delete name;

    if (p != output_channel_names.end()) {
      int index = p->second;

      if ( ++(nOutgoing[index]) > nOutgoingAlloc[index] ) {
	/* we have to enlarge the array of outgoing synapses */
	nOutgoingAlloc[index] += OUTPUTARRAY_INC;
	outgoing[index] = (AnalogSynapse **)realloc(outgoing[index],nOutgoingAlloc[index]*sizeof(AnalogSynapse *));
      }
      outgoing[index][nOutgoing[index]-1] = syn;
      return 0;
    }
    else {
      TheCsimError.add("PhysicalModel::addOutput Invalid output-channel name %s\n",outputname);
      return -1;
    }
  } else {
    TheCsimError.add("PhysicalModel::addOutput can not use %s %i as outgoing element!\n",Outgoing->className(),Outgoing->getId());
    return -1;
  }
}

/** This function is called after parameters are updated. */
int PhysicalModel::updateInternal() {
  return 0;
}

uint32* PhysicalModel::getInputs(int index) {
  uint32 *ci = 0;

  if (incoming[index]) {
    ci = (uint32 *) mxMalloc(sizeof(uint32));
    *ci = incoming[index]->getId();
  }

  return ci;
}

uint32* PhysicalModel::getOutputs(int index) {
  uint32 *ci = 0;

  if (nOutgoing[index] > 0) {
    ci = (uint32 *) mxCalloc (nOutgoing[index], sizeof(uint32));
    for(int i=0;i<nOutgoing[index];i++)
      ci[i] = outgoing[index][i]->getId();
  }

  return ci;
}

//! Register a new input channel
//! \param name Name of the new input channel
//! \return Index of the new channel
int PhysicalModel::register_input_channel(const char *name) {
  string *h = new string(name);
  input_channel_names[*h] = last_input_index;
  delete h;
  return last_input_index++;
}

//! Register a new output channel
//! \param name Name of the new output channel
//! \return Index of the new channel
int PhysicalModel::register_output_channel(const char *name) {
  string *h = new string(name);
  output_channel_names[*h] = last_output_index;
  delete h;
  return last_output_index++;
}


/** Returns the name of an input-channel. */
const char* PhysicalModel::getInputChannelName(int index) {
  if ((index < 0) || (index >= input_channel_names.size())) {
      TheCsimError.add("PhysicalModel::getInputChannelName Invalid input-channel index %i\n",index);
      return 0;
  }
  map<string, int>::iterator p = input_channel_names.begin();
  for (int i=0; i<nIncoming; i++) {
    if (p->second == index)
      return (p->first).c_str();
    p++;
  }

  TheCsimError.add("PhysicalModel::getInputChannelName Could not find channel Nr. %i\n",index);
  return 0;
}

/** Returns the name of an output-channel. */
const char* PhysicalModel::getOutputChannelName(int index) {
  if ((index < 0) || (index >= output_channel_names.size())) {
    TheCsimError.add("PhysicalModel::getOutputChannelName Invalid output-channel index %i\n",index);
    return 0;
  }
  map<string, int>::iterator p = output_channel_names.begin();
  for (int i=0; i<nOutputChannels; i++) {
    if (p->second == index)
      return (p->first).c_str();
    p++;
  }

  TheCsimError.add("PhysicalModel::getOutputChannelName Could not find channel Nr. %i / %i\n",index, nOutputChannels);
  return 0;
}





