/*! \file readout.cpp
**  \brief Implementation of Readout
*/

#include "readout.h"
#include "csimerror.h"
#include "spikingneuron.h"


// Initialize static variables
double* Readout::m_response = 0;
double* Readout::m_target = 0;
pthread_mutex_t Readout::mutex_temp_memory;
int Readout::lReservedMemory = 0;
int Readout::nReadouts = 0;


//! Constructor: Initializes a new readout
Readout::Readout(void) {

  nReadouts++;

  output = 0;

  nRecFields = 0;
  lRecFields = 0;
  recFields  = 0;

  nAnalogFields = 0;
  lAnalogFields = 0;
  analogIndices = 0;

  nSpikingFields = 0;
  lSpikingFields = 0;
  spikingIndices = 0;

  nOutgoing = 0;
  lOutgoing = 0;
  outgoing = 0;
  
  analogFilter = 0;
  spikeFilter = 0;
  algorithm = 0;

  enabled = 1;

  offset = 0;
  range = 1;

  // Initialize the mutex if it is the first readout created
  if (nReadouts == 1)
    pthread_mutex_init(&mutex_temp_memory, NULL);

}


/** Frees the memory. */
Readout::~Readout(void)
{
  pthread_mutex_unlock(&mutex_temp_memory);
  
  nReadouts--;

  for(int i=0;i<nRecFields;i++) {
    delete recFields[i];
    recFields[i] = 0;
  }

  if (recFields) free(recFields); recFields = 0;

  if (analogIndices)
    free(analogIndices);
  analogIndices = 0;

  if (spikingIndices)
    free(spikingIndices);
  spikingIndices = 0;

  if (outgoing)
    free(outgoing);
  outgoing = 0;


  // Free the temp-memory and destroy the mutex if this is the last remaining readout
  if (nReadouts == 0) {
    // pthread_mutex_lock(&mutex_temp_memory);
    if (m_response)
      free(m_response);
    m_response = 0;
    if (m_target)
      free(m_target);
    m_target = 0;

    lReservedMemory = 0;
    
    // pthread_mutex_unlock(&mutex_temp_memory);

    pthread_mutex_destroy(&mutex_temp_memory);
  }
}

//! Connects the readout to a target object which can be a feedback-neuron or a physical model.
int Readout::addOutgoing(Advancable *a) {

  // Look if out output is a neuron
  SynapseTarget *t = dynamic_cast<SynapseTarget *>(a);
  if ( t ) {

    if ( ++nOutgoing > lOutgoing ) {
      // Allocate new memory for the outgoing target
      lOutgoing++;
      outgoing = (SynapseTarget **)realloc(outgoing,lOutgoing*sizeof(SynapseTarget  *));
    }

    // Register new outgoing target
    outgoing[nOutgoing-1] = t;

    return 0;
  } else {
    
    TheCsimError.add("Readout::addOutgoing: object must be a synaptic target; which %s is not!\n",a->className());
    return -1;
  }
  return 0;
}

//! Connects this readout with filters, preprocessors and algorithm objects
int Readout::addIncoming(Advancable *Incoming) {

  // Check if incoming object is an analog filter
  AnalogFilter *af = dynamic_cast<AnalogFilter *>(Incoming);
  if ( af ) {
    setAnalogFilter(af);
    return 0;
  }

  // Check if incoming object is a spike filter
  SpikeFilter *sf = dynamic_cast<SpikeFilter *>(Incoming);
  if ( sf ) {
    setSpikeFilter(sf);
    return 0;
  }

  // Check if incoming object is a preprocessor
  Preprocessor *pre = dynamic_cast<Preprocessor *>(Incoming);
  if ( pre ) {
    appendPreprocessor(pre);
    return 0;
  }

  // Check if incoming object is an algorithm
  Algorithm *alg = dynamic_cast<Algorithm *>(Incoming);
  if ( alg ) {
    setLearningAlgorithm(alg);
    return 0;
  }

  // Else: Invalid object connected
  TheCsimError.add("Readout::addIncoming: Can only directly connect to filters, preprocessors or algorithms!\n");
  return -1;
}




//! Adds a new field to record input data to the readout from object O. 
int Readout::addInputField(csimClass *o, char *fieldname) {
  if ( !o ) {
    TheCsimError.add("Readout::addInputField: NULL pointer specified!\n");
    return -1;
  }

  RecField *f=new RecField;

  f->p = 0; f->type = -1;

  if ( 0 == strcmp(fieldname,"spikes") ) {

    // Special treatment of spikes
    SpikingNeuron *n=dynamic_cast<SpikingNeuron *>(o);
    if ( n ) {
      // This is a spiking neuron, spikes have ID -1
      f->i = -1;

      // Add channel to spikefilter
      if (spikeFilter)
	spikeFilter->addChannel(o);
    } else {
      TheCsimError.add("Readout::addInputField: class %s has no spikes to record.\n",o->className(),fieldname);
      return -1;
    }
  } else {
    // Record anything but a spike-field
    if ( (f->i=o->getFieldId(fieldname)) < 0 ) {
      TheCsimError.add("Readout::addInputField: class %s has no '%s' field.\n",o->className(),fieldname);
      return -1;
    } else {
      f->p    = o->getFieldPointerById((char *)o,f->i);
      f->type = o->getFieldTypeById(f->i);
      if ( o->getFieldSizeById((char *)o,f->i) > 1 ) {
	TheCsimError.add("Readout::addInputField: Recording of fields with size > 1 not supported yet!\n"); 
	return -1;
      }

      // Add channel to analog filter
      if (analogFilter)
	analogFilter->addChannel();
    }
  }

  // Add data to list of recorded fields
  f->o=o;

  add(f);
  
  return 0;
}


//! Add a new field to the recorder for input signals
int Readout::add(RecField *f) {

  if ( ++nRecFields > lRecFields ) {
    // Allocate new memory for the information
    lRecFields += 10;
    recFields = (RecField **)realloc(recFields,lRecFields*sizeof(RecField  *));
  }

  if (f->type > -1) {
    // Index transformation for analog values
    if ( ++nAnalogFields > lAnalogFields) {
      // Allocate new memory for the analog index map
      lAnalogFields += 10;
      analogIndices = (int *) realloc(analogIndices, lAnalogFields * sizeof(int));
    }
    analogIndices[nAnalogFields-1] = nRecFields-1;
  }
  else {
    // Index transformation for spiking values
    if ( ++nSpikingFields > lSpikingFields) {
      // Allocate new memory for the spiking index map
      lSpikingFields += 10;
      spikingIndices = (int *) realloc(spikingIndices, lSpikingFields * sizeof(int));
    }
    spikingIndices[nSpikingFields-1] = nRecFields-1;
  }

  // Allocate new static temporary memory: Always make memory big enough for
  // largest readout (i.e. with most inputs)
  pthread_mutex_lock(&mutex_temp_memory);
  if (nRecFields > lReservedMemory) {
    lReservedMemory += 10;
    m_response = (double *) realloc(m_response, lReservedMemory * sizeof(double));
    m_target = (double *) realloc(m_target, lReservedMemory * sizeof(double));
  }
  pthread_mutex_unlock(&mutex_temp_memory);
  
  // Store new record data
  recFields[nRecFields-1] = f;
  return 0;
}



/** Advances the readout */
int Readout::advance() {
  // No output if readout is disabled
  if (enabled < 1) {
    return 1;
  }

  // No input, nothing to do
  if (nRecFields <= 0) {
    output = offset;
    return 1;
  }

  // ******************* Collect the input *******************

  // ******************************************************************************************************
  // We should not always re-allocate new memory for temporary results, so make it a static member variable
  // ******************************************************************************************************


  pthread_mutex_lock(&mutex_temp_memory);

  double *R = m_response;   // analog inputs
  int i, analog_i, spike_i;

  // Read out all analog inputs
  for (i=0; i<nAnalogFields; i++) {
    analog_i = analogIndices[i];
    switch ( (recFields[analog_i]->type) ) {
    case FLOATFIELD:
      *(R++) = (double) *((float *)(recFields[analog_i]->p));
      break;
    case DOUBLEFIELD:
      *(R++) = *((double *)(recFields[analog_i]->p));
      break;
    case INTFIELD:
      *(R++) = (double) *((int *)(recFields[analog_i]->p));
      break;
    }
  }

  // ******************  Filter the input *******************

  int err_code;

  // Filter the analog input (overwriting the old values)
  if ((analogFilter) && (nAnalogFields > 0)) {
    err_code = analogFilter->filter(m_response, m_response, analogIndices);
    if (err_code < 0) {
      pthread_mutex_unlock(&mutex_temp_memory);
      return -1;
    }
  }


  // Read out and filter all spiking inputs
  if ((spikeFilter) && (nSpikingFields > 0)) {
    err_code = spikeFilter->filter(m_response, m_response, spikingIndices);
    if (err_code < 0) {
      pthread_mutex_unlock(&mutex_temp_memory);
      return -1;
    }
  }

  // ******************  Preprocess the data  ********************
  list<Preprocessor *>::iterator p = pre_processors.begin();
  double *ptr_tmp_data = m_response;
  double *ptr_tmp_target = m_target;
  double *ptr_tmp_swap;

  // Apply each added preprocessor
  while(p != pre_processors.end()) {
    err_code = (*p)->process(ptr_tmp_data, ptr_tmp_target);
    if (err_code < 0) {
      pthread_mutex_unlock(&mutex_temp_memory);
      return -1;
    }

    // Swap between the two temporary storages
    ptr_tmp_swap = ptr_tmp_data;
    ptr_tmp_data = ptr_tmp_target;
    ptr_tmp_target = ptr_tmp_swap;
    p++;
  }

  m_response = ptr_tmp_target;
  m_target = ptr_tmp_data;

  // ****************  Apply the learning algorithm  ************************
  if (algorithm) {
    err_code = algorithm->apply(m_target, &output);
    if (err_code < 0) {
      pthread_mutex_unlock(&mutex_temp_memory);
      return -1;
    }
  }

  pthread_mutex_unlock(&mutex_temp_memory);

  // ****************  Scale the output  **********************
  double a=0, b=0;
  if (algorithm)
    algorithm->getRange(&a, &b);


  if ((b-a) > EPSILON) {
    output = (output-a) / (b-a);
  }


  output = offset + range * output;

  // Write the output to all outgoing targets
  for (i=0; i<nOutgoing; i++) {
    (outgoing[i]->summationPoint) += output;
  }

  return 1;
}


/** Resets the state of the readout and all filters, preprocessors and algorithms. */
void Readout::reset() {
  // Release the mutex
  pthread_mutex_unlock(&mutex_temp_memory);

  // We cannot start with spiking inputs but no spike-filter
  if ((nSpikingFields > 0) && (spikeFilter == 0)) {
    TheCsimError.add("Readout::advance: Cannot start with spiking inputs but without a spike filter!\n"); 
    return;
  }


  // ******************  Test Preprocessors  ********************
  list<Preprocessor *>::iterator p = pre_processors.begin();
  int next_input_dimension = nRecFields;

  // Test each added preprocessor
  while(p != pre_processors.end()) {
    if (*p) {
      if (next_input_dimension != (*p)->getInputRows()) {
	TheCsimError.add("Readout::reset: Input and output dimesions of preprocessors do not agree!\n"); 
	return;
      }
      else {
	next_input_dimension = (*p)->getOutputRows();
      }
    }
    p++;
  }

  if (algorithm) {
    if (next_input_dimension != algorithm->getInputRows()) {
      TheCsimError.add("Readout::reset: Input dimension of algorithm and output dimesion of last preprocessor do not agree!\n"); 
      return;
    }
  }
}

/** Sets a new filter for analog inputs.
    \param af Pointer to new analog filter. If NULL, analog signals will not be filtered. */
void Readout::setAnalogFilter(AnalogFilter *af) {
  if (analogFilter)
    delete analogFilter;
  analogFilter = af;
  if (analogFilter) {
    for (int i=0; i<nAnalogFields; i++)
      analogFilter->addChannel();
  }
}

/** Sets a new filter for spiking inputs.
      \param sf Pointer to new spike-filter. If NULL, spiking signals will not be filtered. */
void Readout::setSpikeFilter(SpikeFilter *sf) {
  if (spikeFilter)
    delete spikeFilter;
  spikeFilter = sf;
  if (spikeFilter) {
    for (int i=0; i<nSpikingFields; i++)
      spikeFilter->addChannel(recFields[spikingIndices[i]]->o);
  }
}

/** Appends a new preprocessor at the end of the preprocessor list. The input will be processed by all preprocessors,
    in the sequence in which they are stored in the preprocessor list.
    \param pp The new preprocessor. 
    \return -1 if an error occured, 1 for success. */
int Readout::appendPreprocessor(Preprocessor *pp) {
  if (pp == 0) {
    TheCsimError.add("Readout::appendPreprocessor: Preprocessor is a NULL pointer!\n"); 
    return -1;
  }

  // Insert the pointer
  pre_processors.push_back(pp);

  return 1;
}

/** Adds a new preprocessor at the beginning of the preprocessor list. The input will be processed by all preprocessors,
    in the sequence in which they are stored in the preprocessor list.
    \param pp The new preprocessor.
    \return -1 if an error occured, 1 for success. */
int Readout::prependPreprocessor(Preprocessor *pp) {
  if (pp == 0) {
    TheCsimError.add("Readout::preprendPreprocessor: Preprocessor is a NULL pointer!\n"); 
    return -1;
  }

  // Insert the pointer
  pre_processors.push_front(pp);

  return 1;
}

/** Inserts a new preprocessor at the specified position within the preprocessor list. The input will be processed by all preprocessors,
      in the sequence in which they are stored in the preprocessor list.
      \param pp The new preprocessor. 
      \param position The position at which the new preprocessor should be inserted.
      \return -1 if the position is invalid, 1 for success. */
int Readout::insertPreprocessor(Preprocessor *pp, int position) {
  if (pp == 0) {
    TheCsimError.add("Readout::insertPreprocessor: Preprocessor is a NULL pointer!\n"); 
    return -1;
  }
  if ((position < 0) || (position > pre_processors.size())) {
    TheCsimError.add("Readout::insertPreprocessor: Invalid index %i!\n", position); 
    return -1;
  }

  // Find position to insert
  list<Preprocessor *>::iterator p = pre_processors.begin();
  for (int i=0; i<position; i++)
    p++;

  // Insert the pointer
  pre_processors.insert(p, pp);

  return 1;
}

/** Returns the length of the preprocessor list.
    \return The length of the preprocessor list. */
int Readout::getNumberPreprocessors() {
  return pre_processors.size();
}

/** Imports a parameterized version of one preprocessor.
    \param ind Index of the preprocessor in the preprocessor list.
    \param rep Representation of the preprocessor parameters.
    \param rep_length Length of the representation vector.
    \return -1 if an error occured, 1 for success. */
int Readout::importPreprocessor(int ind, double *rep, int rep_length) {
  if (pre_processors.empty()) {
    TheCsimError.add("Readout::importPreprocessor: No preprocessors installed!\n"); 
    return -1;
  }
  if ((ind < 0) || (ind >= pre_processors.size())) {
    TheCsimError.add("Readout::importPreprocessor: Invalid index %i!\n", ind); 
    return -1;
  }

  list<Preprocessor *>::iterator p = pre_processors.begin();
  for (int i=0; i<ind; i++)
    p++;

  if (*p)
    return (*p)->importRepresentation(rep, rep_length);
  else {
    TheCsimError.add("Readout::importPreprocessor: Preprocessor %i is a NULL pointer!\n", ind); 
    return -1;
  }
}

/** Exports a parameterized version of one preprocessor.
      \param ind Index of the preprocessor in the preprocessor list.
      \param rep_length Length of the representation vector.
      \return A representation of the preprocessor parameters. */
double* Readout::exportPreprocessor(int ind, int *rep_length) {
  if (pre_processors.empty()) {
    TheCsimError.add("Readout::exportPreprocessor: No preprocessors installed!\n"); 
    return 0;
  }
  if ((ind < 0) || (ind >= pre_processors.size())) {
    TheCsimError.add("Readout::exportPreprocessor: Invalid index %i!\n", ind); 
    return 0;
  }

  list<Preprocessor *>::iterator p = pre_processors.begin();
  for (int i=0; i<ind; i++)
    p++;

  if (*p)
    return (*p)->exportRepresentation(rep_length);
  else {
    TheCsimError.add("Readout::exportPreprocessor: Preprocessor %i is a NULL pointer!\n", ind); 
    return 0;
  }
}

/** Returns a string description of the input/output format of one preprocessor.
    \param ind Index of the preprocessor in the preprocessor list.
    \return A string describing the input/output format of the preprocessor. */
string Readout::getPreprocessorFormatDescription(int ind) {
  if (pre_processors.empty()) {
    TheCsimError.add("Readout::getPreprocessorFormatDescription: No preprocessors installed!\n"); 
    return "";
  }
  if ((ind < 0) || (ind >= pre_processors.size())) {
    TheCsimError.add("Readout::getPreprocessorFormatDescription: Invalid index %i!\n", ind); 
    return "";
  }

  list<Preprocessor *>::iterator p = pre_processors.begin();
  for (int i=0; i<ind; i++)
    p++;

  return (*p)->getFormatDescription();
}

/** Sets a new learning algorithm. This algorithm computes the output of the readout.
    \param learner A pointer to the algorithm object.
    \return -1 if an error occured, 1 for success. */
int Readout::setLearningAlgorithm(Algorithm *learner) {
  if (learner == 0) {
    TheCsimError.add("Readout::setLearningAlgorithm: Input is a NULL pointer!\n"); 
    return -1;
  }

  // First free the memory, then store the new algorithm
  if (algorithm)
    delete algorithm;
  algorithm = learner;
}

/** Imports a parameterized version of the current learning algorithm.
      \param rep Representation of the learning algorithm parameters.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
int Readout::importAlgorithm(double *rep, int rep_length) {
  if (rep == 0) {
    TheCsimError.add("Readout::importAlgorithm: Input is a NULL pointer!\n"); 
    return -1;
  }
  if (algorithm == 0) {
    TheCsimError.add("Readout::importAlgorithm: No algorithm is set!\n"); 
    return -1;
  }

  return algorithm->importRepresentation(rep, rep_length);
}

  /** Exports a parameterized version of the current learning algorithm.
      \param rep_length Length of the representation vector.
      \return A representation of the learning algorithm parameters. */
double* Readout::exportAlgorithm(int *rep_length) {
  if (algorithm)
    return algorithm->exportRepresentation(rep_length);
  else {
    TheCsimError.add("Readout::exportAlgorithm: No algorithm is set!\n"); 
    return 0;
  }
}

/** Returns a string description of the input/output format of the current learning algorithm.
    \return A string describing the input/output format of the current learning algorithm. */
string Readout::getAlgorithmFormatDescription() {
  if (algorithm)
    return algorithm->getFormatDescription();
  else {
    TheCsimError.add("Readout::getAlgorithmFormatDescription: No algorithm is set!\n"); 
    return "";
  }
}


/** Set the range of output values.
    \param lower_bound Lower bound for output values.
    \param upper_bound Upper bound for output values. */
void Readout::setRange(double lower_bound, double upper_bound) {
  double tmp;
  if (lower_bound > upper_bound) {
    tmp = lower_bound;
    lower_bound = upper_bound;
    upper_bound = tmp;
  }

  offset = lower_bound;
  range = upper_bound - lower_bound;
}
  


/** Get the Ids of the Preprocessors.
    \param idx Pointer to the memory where to store the Ids. */
void Readout::getPreprocessorIDs(uint32 *idx) {
  list<Preprocessor *>::iterator p = pre_processors.begin();

  while (p != pre_processors.end()) {
    *idx++ = (*p)->getId();
    p++;
  }
}

