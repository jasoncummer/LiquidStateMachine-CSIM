/*! \file network.h
**  \brief Class definition of Network
*/

#ifndef _NETWORK_H_
#define _NETWORK_H_

//#include "csimclass.h"
#include "csimlist.h"
#include "spikingsynapse.h"

class Forceable;
class Neuron;
class Synapse;
class Teacher;
class csimRecorder;
/* ************** BEGIN MICHAEL PFEIFFER **************** */
class Readout;
class Preprocessor;
class Algorithm;
class PhysicalModel;
/* ************** END MICHAEL PFEIFFER **************** */
struct csimInputChannel;

//! Container class for a network of \link #Neuron neurons \endlink and \link #Synapse synapses\endlink.
/**
 ** Container class for a network of \link #Neuron neurons \endlink and \link #Synapse synapses\endlink.
 */
class Network {

public:

  //! Constructor
  Network(void);

  //! Destructor
  virtual ~Network();

  //! Add a new object to the network. Note that we <b>do not check</b> if it was already added before.
  virtual int addNewObject(Advancable *a);

  //! Return a pointer to the object in the network identified by \c idx
  Advancable *getObject(uint32 idx);


  //! Setup an information flow from source to destionation using indexes
  int connect(uint32 destination, uint32 source);
  int connect(uint32 dst, uint32 *src, uint32 n);
  int connect(uint32 *dst, uint32 src, uint32 n);
  int connect(uint32 *dst, uint32 *src, uint32 n);
  int connect(uint32 recorderIdx, uint32 *objIdx, uint32 n, char *fieldName);
  int connect(uint32 *postIdx, uint32 *preIdx, uint32 *synIdx , uint32 n);

  //! Call reset for each object in the network
  int reset(void);

  //! Simulate the network for some time steps with an array of given inputs.
  int simulate(unsigned long nSteps, csimInputChannel *inputSignal, int ni, csimInputChannel *teacherSignal, int nt);

  //! Count the total number of spikes occured during the simulation (starting at time \f$t=0\f$).
  unsigned long totalSpikeCount(void);

  //! Return all spikes occured during the simulation (starting at time \f$t=0\f$).
  void getSpikes(uint32 *idx, double *times);

  //! Return the number of elements of a given class within the network.
  int getNumOf(char *className);

  //! Return total number of objects in the network.
  int getNumObjects(void) {return objectList.n;}

  //! Set field of given objects to given value 
  int setParameter(uint32 *idx, int nIdx, char *paramName, double *value, int m, int n);

  //! Get the values of a given fields of severl objects
  int getParameter(uint32 *idx, int n, char *paramName, double **value, int *m);

  //! Update all internal variables of all object by calling object->fieldChangeNotify();
  int updateObjects(void);

  //! This function will be called by any SpikingNeuron if it wants to transmit a spike.
  // void preSpikeNotify(SpikingSynapse *s);

  //! This function will be called by any SpikingNeuron if it wants to transmit a spike.
  void ScheduleSpike(SpikingSynapse *s, pSpikeHandler h, double delay);

  //! Print out a list of all objects
  int listObjects(bool fields);

  /*! @name Methods for setting and getting global fields */
  //@{
  //! Set the the given field (\a fieldName ) to value \a v.
  int setField(char *fieldName, double v);

  //! Get the value of the given field.
  int getField(char *fieldName, double *v);

  //! Print the values of all fields.
  void printFields(void);

  //! Return the number of accessible fields
  int getFieldArraySize(void);
  
  //! Allocate and retunr an array containing all fields
  double *getFieldArray(void);

  //! Set fields specified by the array \a p
  void setFieldArray(double *p);
  //@}


  /* ******************* BEGIN MICHAEL PFEIFFER ******************** */
  //! Import preprocessors or algorithm
  int importObject(uint32 *idx, int nIdx, double *value, int n);

  //! Export preprocessors or algorithm
  int exportObject(uint32 *idx, int nIdx, double ***value, int **m);

  /* ******************* END MICHAEL PFEIFFER ******************** */

  //! The integration time step. \internal [readwrite; units=sec; range=(1e-12,1);]
  double dt;

  //! The seed value for the random generator. \internal [units=; range=(0,MAXINTEGER);]
  int randSeed;

  //! How much stuff should be written to the console window. \internal [readwrite; units=; range=(0,20);]
  int verboseLevel;

  //! Number of threads to use for simulation
  int nThreads;

  //! Flag: 1 ... always output spikes as last cell element, 0 do not output spikes.  \internal [readwrite; units=; range=(0,1)]
  int spikeOutput;

  //! The current virtual (simulation) time. \internal [readonly; units=sec;]
  double t;

  //! The current time step := t/dt; \internal [hidden; units=steps;]
  unsigned long step;

private:
  friend class Teacher;
  friend class MexNetwork;

  //! This function will be called by a Teacher an object is added to this teacher.
  int moveToTeached(Forceable *f);

  //! This function processes the input array and assignes the inputs to the specified objects.
  int assignInputs(csimInputChannel *input, int n);

  //! This function processes the input array and removes the inputs from the specified objects.
  int unAssignInputs(csimInputChannel *input, int n);

  //! Setup an information flow from source to destionation
  int connect(Advancable *destination, Advancable *source);
  //int connect(Advancable *destination, char *ip, Advancable *source, char *op);

  //! Set to 1 during a call to reset()
  int isReset;

  //! Flag: Must be set to one if dt is changed.
  bool dtChanged;

  //! Flag: 1 ... some fields of at least one object in the network have been changed
  bool fieldsOfSomeObjectChanged;

  //! List of all neurons .
  csimList<Neuron,100> allNeuronList;

  //! List of NOT teached objects
  csimList<Forceable,100> notTeachedList;

  //! List of TEACHED objects.
  csimList<Forceable,100> teachedList;

  //! List of all synapses.
  csimList<Synapse, 10000> synapseList;

  csimList<Synapse, 10000> activeSyn;

  //! List of teachers.
  csimList<Teacher,2> teacherList;

  //! List of recorders.
  csimList<csimRecorder,2> recorderList;

  /* *************** BEGIN MICHAEL PFEIFFER ************* */
  //! List of readouts.
  csimList<Readout,2> readoutList;

  //! List of preprocessors.
  csimList<Preprocessor,2> preprocessorList;

  //! List of algorithms.
  csimList<Algorithm,2> algorithmList;

  //! List of models.
  csimList<PhysicalModel,2> modelList;
  /* *************** END MICHAEL PFEIFFER ************* */

  //! List of all advanceable objects (all neurons, synapses, teachers, and recorders)
  csimList<Advancable,20000> objectList;

  //! The integration time step in microseconds
  long DT_us;

  //! Last timepoint when we synced to real-time
  long lastSyncTime;

  //! Structure for memory management of the event/spike driven part of the simulation
  struct event {
    //! Synapse which reveives a spike
    SpikingSynapse *s; 

    //! pointer do spike handler
    pSpikeHandler h;

    //! Pointer to next elemet in various lists
    event *next;
  };
        
  csimList<event, 20> allocList;

  event *recycledEvent;
  event *freeList;
  int lfreeList;
  int freeIdx;
  unsigned allocIdx;

  int ldelayList;
  event **delayList;
  int delayIdx;

protected:

  unsigned nDst;
  unsigned lDst;
  uint32 *dst;

  unsigned nSrc;
  unsigned lSrc;
  uint32 *src;

  unsigned nRecMem;
  unsigned lRecMem;
  char *RecMem,*pRecMem;

  /* ******** BEGIN MICHAEL PFEIFFER ******** */
  unsigned nReadoutMem;
  unsigned lReadoutMem;
  char *ReadoutMem,*pReadoutMem;

  unsigned nModelMem;
  unsigned lModelMem;
  char *ModelMem,*pModelMem;
  /* ******** END MICHAEL PFEIFFER ******** */
};

#endif
