/*! \file readout.h
**  \brief Class definition of Readout
*/

#ifndef _READOUT_H_
#define _READOUT_H_

#include "advanceable.h"
#include "analogfilter.h"
#include "spikefilter.h"
#include "preprocessor.h"
#include "algorithm.h"
#include <list>
#include <string>
#ifndef _WIN32
#include <pthread.h>
#else
 #define pthread_mutex_t int
 #define pthread_mutex_init(a,b)
 #define pthread_mutex_lock(a)
 #define pthread_mutex_unlock(a)
 #define pthread_mutex_destroy(a)
#endif
#include "csimclass.h"
#include "modelinput.h"

#define EPSILON 10e-20



//! Base class of all readouts
/** This class implements readouts within the simulation. Readouts take the responses of the
 ** liquid and calculate a new function with these inputs. Readouts can be trained to
 ** approximate some target function.
 ** Readouts can be connected to physical models or feedback neurons for closed-loop simulations.
 ** Therefore the interface of readouts is similar to that of synapses.
 ** The output of a readout can be recorded.
  */
class Readout : public Advancable, public ModelInput {

  DO_REGISTERING

 public:

  //! Constructor: Initializes a new readout
  Readout(void);

  //! Destructor: Frees the memory allocated by the readout
  ~Readout(void);

  //! Advances the readout
  virtual int advance();

  //! Connects this readout with filters, preprocessors and algorithm objects
  virtual int addIncoming(Advancable *Incoming);

  //! Connects the readout to a target object which can be a feedback-neuron or a physical model.
  virtual int addOutgoing(Advancable *a);

  //! Adds a new field to record input data to the readout from object O.
  virtual int addInputField(csimClass *O, char *fieldname);

  /** Resets the state of the readout and all filters, preprocessors and algorithms. */
  virtual void reset();

  /** Sets a new filter for analog inputs.
      \param af Pointer to new analog filter. If NULL, analog signals will not be filtered. */
  void setAnalogFilter(AnalogFilter *af);

  /** Sets a new filter for spiking inputs.
      \param sf Pointer to new spike-filter. If NULL, spiking signals will not be filtered. */
  void setSpikeFilter(SpikeFilter *sf);

  /** Appends a new preprocessor at the end of the preprocessor list. The input will be processed by all preprocessors,
      in the sequence in which they are stored in the preprocessor list.
      \param pp The new preprocessor.
      \return -1 if an error occured, 1 for success. */
  int appendPreprocessor(Preprocessor *pp);

  /** Adds a new preprocessor at the beginning of the preprocessor list. The input will be processed by all preprocessors,
      in the sequence in which they are stored in the preprocessor list.
      \param pp The new preprocessor.
      \return -1 if an error occured, 1 for success. */
  int prependPreprocessor(Preprocessor *pp);

  /** Inserts a new preprocessor at the specified position within the preprocessor list. The input will be processed by all preprocessors,
      in the sequence in which they are stored in the preprocessor list.
      \param pp The new preprocessor.
      \param position The position at which the new preprocessor should be inserted.
      \return -1 if the position is invalid, 1 for success. */
  int insertPreprocessor(Preprocessor *pp, int position);

  /** Returns the length of the preprocessor list.
      \return The length of the preprocessor list. */
  int getNumberPreprocessors();

  /** Imports a parameterized version of one preprocessor.
      \param ind Index of the preprocessor in the preprocessor list.
      \param rep Representation of the preprocessor parameters.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  int importPreprocessor(int ind, double *rep, int rep_length);

  /** Exports a parameterized version of one preprocessor.
      \param ind Index of the preprocessor in the preprocessor list.
      \param rep_length Length of the representation vector.
      \return A representation of the preprocessor parameters. */
  double* exportPreprocessor(int ind, int *rep_length);

  /** Returns a string description of the input/output format of one preprocessor.
      \param ind Index of the preprocessor in the preprocessor list.
      \return A string describing the input/output format of the preprocessor. */
  string getPreprocessorFormatDescription(int ind);

  /** Sets a new learning algorithm. This algorithm computes the output of the readout.
      \param learner A pointer to the algorithm object.
      \return -1 if an error occured, 1 for success. */
  int setLearningAlgorithm(Algorithm *learner);

  /** Imports a parameterized version of the current learning algorithm.
      \param rep Representation of the learning algorithm parameters.
      \param rep_length Length of the representation vector.
      \return -1 if an error occured, 1 for success. */
  int importAlgorithm(double *rep, int rep_length);

  /** Exports a parameterized version of the current learning algorithm.
      \param rep_length Length of the representation vector.
      \return A representation of the learning algorithm parameters. */
  double* exportAlgorithm(int *rep_length);

  /** Returns a string description of the input/output format of the current learning algorithm.
      \return A string describing the input/output format of the current learning algorithm. */
  string getAlgorithmFormatDescription();


  /** Set the range of output values.
      \param lower_bound Lower bound for output values.
      \param upper_bound Upper bound for output values. */
  void setRange(double lower_bound, double upper_bound);



  //! Get the Id of the Analog-Filter
  inline uint32 getAnalogFilterID(void)  { if (analogFilter) return analogFilter->getId(); else return 0; }

  //! Checks whether an Analog-Filter exists
  inline bool hasAnalogFilter(void) { return (analogFilter != 0); }

  //! Get the Id of the Spike-Filter
  inline uint32 getSpikeFilterID(void) { if (spikeFilter) return spikeFilter->getId(); else return 0; };

  //! Checks whether a Spike-Filter exists
  inline bool hasSpikeFilter(void) { return (spikeFilter != 0); }

  //! Get the Id of the Algorithm
  inline uint32 getAlgorithmID(void) { if (algorithm) return algorithm->getId(); else return 0; };

  //! Checks whether a Spike-Filter exists
  inline bool hasAlgorithm(void) { return (algorithm != 0); }

  //! Get the Ids of the Preprocessors
  void getPreprocessorIDs(uint32 *idx);


  //! Flag: 0 ... readout disabled, 1 ... readout enabled [range=(0,1)];
  int enabled;


  //! Offset of output values
  double offset;

  //! Range of output values
  double range;


  // We must also implement some recorder interface, because we want to use
  // not only the psr of neurons as input, but also other fields


 protected:

  //! Stores information about a field to record from
  struct RecField {
    //! Index of field to record
    short      i;

    //! Object to record from
    csimClass *o;

    //! Pointer to field which should be recorded
    char *p;

    //! Type of field to record
    short type;
  };

  //! Add a new field to the recoder
  int add(RecField *f);


  //! Number of fields recorded \internal [hidden;]
  int nRecFields;

  //!  Number of fields allocated \internal [hidden;]
  int lRecFields;

  //! Array of fieldpointers \internal [hidden;]
  RecField **recFields;

  //! Number of analog (i.e. nonspiking fields) recorded \internal [hidden;]
  int nAnalogFields;
  //! Number of analog (i.e. nonspiking fields) allocated \internal [hidden;]
  int lAnalogFields;

  //! Array of indices for the analog fields \internal [hidden;]
  int *analogIndices;

  //! Number of spiking fields recorded \internal [hidden;]
  int nSpikingFields;
  //! Number of spiking fields allocated \internal [hidden;]
  int lSpikingFields;

  //! Array of indices for the spiking fields \internal [hidden;]
  int *spikingIndices;



  //! Pointers to outgoing targets \internal [hidden;]
  SynapseTarget **outgoing;

  //! Number of outgoing targets \internal [hidden;]
  int nOutgoing;
  //! Number of outgoing targets allocated \internal [hidden;]
  int lOutgoing;




  //! Current output of the readout (defined in ModelInput)
  //  double output;

  //! List of preprocessors \internal [hidden;]
  list<Preprocessor *> pre_processors;

  //! Filter for analog input \internal [hidden;]
  AnalogFilter *analogFilter;

  //! Filter for spiking input \internal [hidden;]
  SpikeFilter *spikeFilter;

  //! Learned algorithm \internal [hidden;]
  Algorithm *algorithm;


  //! Static working memory for filters and preprocessors \internal [hidden;]
  static double* m_response;
  //! Static working memory for filters and preprocessors \internal [hidden;]
  static double* m_target;

  //! Length of the reserved memory \internal [hidden;]
  static int lReservedMemory;

  //! Mutex for controlling access to shared memory \internal [hidden;]
  static pthread_mutex_t mutex_temp_memory;

  //! Number of readouts created \internal [hidden;]
  static int nReadouts;

};


#endif
