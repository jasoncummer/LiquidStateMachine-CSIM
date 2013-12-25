/*! \file recorder.h
**  \brief Class definition of CbNeuron
*/

#ifndef _RECORDER_H_
#define _RECORDER_H_

#include "advanceable.h"

//! Implements an object which can record fields from other objects
class csimRecorder : public Advancable {

 public:
  csimRecorder(void);
  virtual ~csimRecorder();

  virtual void reset(void);
  virtual int advance(void);
  virtual int updateInternal(void);

  // Adds a new field to record from object O. 
  virtual int addFieldToRecord(csimClass *O, char *fieldname);

  virtual int addOutgoing(Advancable *){return 0;};
  virtual int addIncoming(Advancable *){return 0;};

  //! The timestep at which an recording should be done (no meaning if recording spikes). [units=sec; range=(0,100)]
  double dt;

  //! Provide your best guess how long the network will be simulated (in simulation time). [units=sec; range=(0,100)]
  double Tprealloc;

  //! Flag: 0 ... recorder disabled, 1 ... recoder enabled [range=(0,1)];
  int enabled;

 protected:
  //! How many values per field we have actually recorded \internal [hidden;]
  int nRecordSteps;

  //! Stores information about a field to recored from
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

  //! Number of fields recorded \internal [hidden;]
  int nRecFields;

  //!  Number of fields allocated \internal [hidden;]
  int lRecFields;

  //! Array of fieldpointers \internal [hidden;]
  RecField **recFields;

  //! Number of analog (i.e. nonspiking fields) recorded \internal [hidden;]
  int nAnalogFields;

  //! [hidden]
  double *values;          // pointer to memory where we write the recorded stuff
  //! [hidden]
  int    nValues;
  //! [hidden]
  int    lValues;
  //! [hidden]
  int mod;                 // every mod time steps we actually do a recording

 private:
  double *v;               // current position to write to

  int nStepsToRec;         // how much time steps until the next actual recording is done 

  //! Add a new field to the recoder
  int add(RecField *f);
 
};

#endif
