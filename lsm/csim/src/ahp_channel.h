#ifndef _AHP_CHANNEL_H_
#define _AHP_CHANNEL_H_

#include "ionchannel.h"


//! AHP model with constant activation gating function and constant step increase of the calcium concentration
/**
 *  Reference:
 *  Fuhrmann G, Markram H, Tsodyks M.
 *  Spike frequency adaptation and neocortical rhythms.
 *  J Neurophysiol. 2002 Aug;88(2):761-70.
 */
class AHP_Channel : public IonChannel {

  DO_REGISTERING
  
public:
  
  AHP_Channel(void);

  virtual ~AHP_Channel(void) {};

  virtual void reset(void);

  virtual int updateInternal(void);

  virtual int advance(void);

  virtual void membraneSpikeNotify(double );

  virtual int addIncoming(Advancable *Incoming);

  virtual int addOutgoing(Advancable *Outgoing);

  //! The maximum conductance of the channel; [units=S; range=(0,1); readwrite;]
  float Gbar;

  //! Fraction of the open conductance; [range=(0,1); readonly;]
  float n;

  //! Constant step increase in n for each spike; [range=(0,1); readwrite;]
  float u;

  //! Time constant for the deactivation of the current;  [range=(0,1e15); readwrite;]
  float Ts;

protected:

  //! Constant for the exponential Euler integration step; \internal [hidden]
  float C1;

private:

};




#endif
