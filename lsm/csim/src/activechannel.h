#ifndef __ACTIVECHANEL_H__
#define __ACTIVECHANEL_H__

#include "ionchannel.h"
#include "membranepatchsimple.h"

class IonGate;

//! Base Class for all active ionic channels using ion gates
/**
 ** \model 
 **
 ** Generic active channel with arbitray number of \link IonGate ion
 ** gates \endlink. \link #g \f$g(t,V_m)\f$ \endlink is computed as
 ** \f[ g(t,V_m) = \bar{g} \prod_{i} P_i(t,V_m) \f] where \link #Gbar
 ** \f$\bar{g}\f$ \endlink is the maximal conductance, and \f$P_i(t,V_m)
 ** \in [0,1]\f$ is fraction of \f$i\f$-type gates currently open.
 **
 ** Note that no specific gates are modeled. Instead arbitrary \link
 ** IonGate ion gate objects \endlink can be connected to the channel.
 **
 ** \endmodel
 **
 ** \inout
 **
 ** Same as for class IonChannel .
 **
 ** \endinout
 **
 ** */
class ActiveChannel : public IonChannel {

  DO_REGISTERING
  
public:
  
  ActiveChannel(void);

  virtual ~ActiveChannel(void);

  //! The maximum conductance of the channel; [units=S; range=(0,1); readwrite;]
  float Gbar;

  virtual int updateInternal(void);

  virtual void reset(void);

  virtual int advance(void);

  virtual double gInfty(void);

  virtual void membraneSpikeNotify(double ) { };

  virtual int addIncoming(Advancable *Incoming);

  virtual int addOutgoing(Advancable *Outgoing);

protected:
  
  virtual void addGate(IonGate *gate);

  //! The number of ion gates \internal [hidden;]
  int nGates;

  //! Memory currently allocated for ion gates \internal [hidden;]
  int lGates;
  IonGate **gates;

private:

};

#endif
