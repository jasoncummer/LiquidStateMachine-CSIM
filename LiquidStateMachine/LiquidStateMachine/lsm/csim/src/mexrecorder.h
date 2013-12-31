#ifndef __MEXRECORDER_H__ 
#define __MEXRECORDER_H__ 

#include <mex.h>
#include "recorder.h"


//! Records fields from arbitrary objects during simulation
/**
 **
 ** This class allows you to record traces of any fields of any object
 ** during the simulation. The recorded traces of an Recorder with
 ** handle \c rec_idx can be obtained via \code
 ** R=csim('get',rec_idx,'traces'); \endcode The ecxact form of \c R
 ** depends on the flag \c #commonChannels (see below). Note that the
 ** traces returned always start at time $t=0$ and are recorded at an
 ** interval of \c #dt.
 **
 ** In addition a recorder can also record spikes from spike emitting
 ** objects. Via a command like \code
 ** csim('connect',rec_idx,neuron_idx,'spikes'); \endcode the Recorder
 ** with handle \c rec_idx is set up to record the spikes form the
 ** spike emitting objects with handles \c neuron_idx.
 **
 ** \b commonChannels=0 In this case the Matlab array \c R
 ** is a struct array with the only field \c channel which is
 ** in turn a struct array with the following fields:
 **
 **  - \c R.channel(j).idx : handle of the object from which field
 **  the data was recorded
 **
 **  - \c R.channel(j).spiking : binary flag (0/1) which determines
 **  if \c data should be interpreted as spike times or as an analog
 **  signal
 **   
 **  - \c R.channel(j).dt : time discretization; for analog signals
 **  only
 **  
 **  - \c R.channel(j).data : signal data : vector of the analog values
 **  or spike times.
 ** 
 **  - \c R.channel(j).fieldName : name of the recorded field
 **
 ** \b commonChannels=1 In this case **\c R has two fields
 ** (WARNING: no spikes are returned):
 **
 **  - \c R.data : A double array where \c R.data(j,s) is the \c s -th
 **  recorded value of the \c j -th field.
 **  
 **  - \c R.info : A struct array where \c R.info(j).idx is the handle of
 **  the object from which the field \c R.info(j).fieldName is recorded.  */
class Recorder : public csimRecorder {

  DO_REGISTERING 

public:
  Recorder(void);

  mxArray *getMxStructArray(void);

  //! Flag: 1 ... output all channels in one matrix (WARNING: no spikes are returned yet), 0 ... output each recorded field as seperate channel  [readwrite; range=(0,1); units=;]
  int commonChannels;

protected:
  mxArray *getMxCommonTraces(void);
  mxArray *getMxSeparateTraces(void);

};

//! Alias for Recorder for backwards compatibility
class MexRecorder : public Recorder {

  DO_REGISTERING 

};

#endif
