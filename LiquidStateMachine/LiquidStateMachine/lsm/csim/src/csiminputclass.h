#ifndef __CSIMINPUTCLASS_H__
#define __CSIMINPUTCLASS_H__

#include "globaldefinitions.h"
#include "csimlist.h"

//! Structure representing an analog or spiking signal channel
struct csimInputChannel {
  //! Length of data array
  int length;

  //! Index to the next data point (relevant only for spiking channels)
  int next;

  //! Discretization of the signal. dt=-1 for spiking channels
  double dt;

  //! Needed only for spiking input channels
  double lastSpikeTime;

  //! Pointer to the data
  double *data;

  //! Arry of idx to connect channel to
  uint32 *idx;
  int    nIdx;
};

//! Base class for all input channel receiving classes like AnalogInputNeuron or SpikingInputNeuron
class csimInputClass {
public:
  csimInputClass() { channelIdChecked=0; };
  virtual ~csimInputClass() {};
  inline int nChannels(void) { return channel.n; }
  virtual int addInputChannel(csimInputChannel *c)=0;
  void clearInputChannels(void);
protected:
  inline csimInputChannel* getChannel(uint32 c);
  void addChannel(csimInputChannel *c);
private:
  csimList<csimInputChannel,1> channel;
  bool channelIdChecked;
};

//! Base class for all spiking input channel receiving classes like SpikingInputNeuron or SpikingTeacher
class csimSpikingInputClass : public csimInputClass {
public:
  int addInputChannel(csimInputChannel *c);
  int nextValue(int i);
};

//! Base class for all analog input channel receiving classes like AnalogInputNeuron or AnalogTeacher
class csimAnalogInputClass : public csimInputClass {
public:
  int addInputChannel(csimInputChannel *c);
  double nextValue(int i);
};

#endif
