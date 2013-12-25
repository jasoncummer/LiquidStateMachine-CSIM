/** \file physicalmodel.h
**  \brief Base class of all phyiscal models for CSIM simulations.
*/

#ifndef _PHYSICALMODEL_H_
#define _PHYSICALMODEL_H_

#include <map>
#include <string>
#include "csimerror.h"
#include "csimclass.h"
#include "advanceable.h"
#include "modelinput.h"
#include "analogsynapse.h"
#include "readout.h"

using namespace std;

//! Base class of all physical models to be used in CSIM simulations.
/** These models take input from readouts and produce one or more outputs to analog synapses. Within a transform function you can also receive input from any external source. */
class PhysicalModel : public Advancable {

 public:
  /** Constructs a new physical model.
      \param input_channels How many input channels are there?
      \param output_channels How many output channels are there? */
  PhysicalModel(int input_channels, int output_channels);

  virtual ~PhysicalModel(void);

  //! Calculates the next output of the physical model.
  virtual int advance(void);

  //! This method will be called if object \a Incoming wants to send information to \a this object
  virtual int addIncoming(Advancable *Incoming) {
    return 0;
  }

  //! Connect a readout output to a named input of the model
  /** \param inputname Name of the input channel
      \param Incoming Pointer to the input channel
    */
  virtual int addInput(Advancable *Incoming, char *inputname);

  //! This method will be called if \a this object wants to send information to object Outgoing
  virtual int addOutgoing(Advancable *Outgoing) {
    return 0;
  }

  //! Connect an output channel of the model to a synapse
  /** \param outputname Name of the output channel
     \param Outgoing Pointer to the output channel
    */
  virtual int addOutput(Advancable *Outgoing, char *outputname);


  /** Transforms the current inputs to new output values.
      \param I Array of pointers to input values from the readouts.
      \param O Array of output values.
      \return -1 if an error occured, 1 for success. */
  virtual int transform(double** I, double* O) = 0;

  /** Resets the information stored within the model. */
  virtual void reset() {};

  /** This function is called after parameters are updated. */
  virtual int updateInternal();

  inline uint32 nInChannels(void)  { return nIncoming; }
  inline uint32 nOutChannels(void) { return nOutputChannels; }
  inline uint32 nOutputs(int i) { return nOutgoing[i]; }
  uint32* getInputs(int index);
  uint32* getOutputs(int index);

  /** Returns the names of the input- and output - channels. */
  const char* getInputChannelName(int index);
  const char* getOutputChannelName(int index);

 protected:

  //! Register a new input channel
  //! \param name Name of the new input channel
  //! \return Index of the new channel
  int register_input_channel(const char *name);

  //! Register a new output channel
  //! \param name Name of the new output channel
  //! \return Index of the new channel
  int register_output_channel(const char *name);

  //! A list of incoming model-inputs \internal [hidden]
  Advancable **incoming;

  //! Number of inputs \internal [readonly; units=;]
  int nIncoming;

  //! Current size  of array (in number of inputs) allocated for inputs \internal [hidden]
  int nIncomingAlloc;

  //! An array of outgoing synapses \internal [hidden;]
  AnalogSynapse ***outgoing;

  //!  Number of outgoing synapses per channel \internal [hidden]
  int *nOutgoing;

  //! Current size  of array (in number of synapses) allocated for outgoing synapses per channel \internal [hidden]
  int *nOutgoingAlloc;

  //! Number of output channels \internal [readonly; units=;]
  int nOutputChannels;

  //! Mapping of input-channel names to indices \internal [hidden]
  map<string, int> input_channel_names;

  //! Mapping of output-channel names to indices \internal [hidden]
  map<string, int> output_channel_names;

 private:
  //! Storage for input value-pointers \internal [hidden;]
  double **inputs;
  
  //! Storage for output values \internal [hidden;]
  double *outputs;

  //! Last registered input index \internal [hidden;]
  int last_input_index;
  //! Last registered output index \internal [hidden;]
  int last_output_index;
};

#endif
