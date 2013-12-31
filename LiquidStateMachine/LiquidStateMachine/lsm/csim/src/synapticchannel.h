#ifndef __SYNAPTICCHANEL_H__
#define __SYNAPTICCHANEL_H__

#include "ionchannel.h"
#include "synapsetarget.h"

class Synapse;

//! IonChannel which transforms spikes into conductance changes
class SynapticChannel : public IonChannel, public SynapseTarget {

  //  DO_REGISTERING
  
public:
  
  SynapticChannel(void);

  virtual ~SynapticChannel(void);

  //! The maximum conductance of the channel; [units=S; range=(0,1); readwrite;]
  float Gbar;
  
  virtual void reset(void);
  
  virtual int advance(void);

  virtual double gInfty(void) { return 0.0; };

  virtual void membraneSpikeNotify(double ) { };

  virtual int addIncoming(Advancable *Incoming);

  virtual int addOutgoing(Advancable *Outgoing);
 
  virtual uint32 getPostId(void) { return getId(); }

protected:
  
  void addSynapse(Synapse *S);
  int nSynapses;
  int lSynapses;
  Synapse **synapses;

};

#endif
