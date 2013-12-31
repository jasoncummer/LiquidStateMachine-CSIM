/*! \file ionchannel.h
**  \brief Class definition of IonChannel
*/

#ifndef _IONCHANNEL_H_
#define _IONCHANNEL_H_

#include "globaldefinitions.h"
#include "advanceable.h"
#include "membranepatchsimple.h"

class CbNeuron;
class MembranePatchSimple;

//! Base class for all ionic channels.
/**
 ** \latexonly \subsubsection*{The Model} \endlatexonly
 ** \htmlonly <h3>Model</h3> \endhtmlonly
 **
 ** Since IonChannel is the base class for all ionic channels it does
 ** not implement any specific channel model.
 **
 ** \endmodel
 **
 **
 ** \latexonly \subsubsection*{Input and output signals} \endlatexonly
 ** \htmlonly <h3>Input and output signals</h3> \endhtmlonly
 **
 ** The output signals of an IonChannel object are
 ** the reversal potential \link #Erev \f$E_{rev}\f$ \endlink and
 ** the time (and voltage) dependent conductance \link #g
 ** \f$g(t,V_m)\f$ \endlink.
 **
 ** As input signal an IonChannel needs the membrane voltage
 ** \f$V_m\f$ of the corresponding CbNeuron, MembranePatchSimple, or
 ** AnalogNeuron to which the object is connected.
 **
 ** To adjust to arbitrary resting potentials an IonChannel also needs
 ** access to the resting potential \f$V_{resting}\f$
 ** of the corresponding object.
 **
 ** */
class IonChannel : public Advancable {

 public:

  // The constructor
  IonChannel(void) { Erev=0; g=0; membrane=0;};

  // The destructor
  virtual ~IonChannel(void) {} ;

  virtual void reset(void)=0;

  virtual int advance(void)=0;

  //!If the object to which the channel belongs emmits a spike this function gets called.
  virtual void membraneSpikeNotify(double t)=0; // this is a spiking input!

  //! This function has to return the value of \f$g(t,V_m)\f$
  //! for \f$t \rightarrow \infty\f$ if the membrane voltage \f$V_m\f$ is clamped to
  //! \c Vclamp .
  virtual double gInfty(void) {return 0;}

  virtual int addIncoming(Advancable *Incoming);

  virtual int addOutgoing(Advancable *Outgoing);

  //! The reversal potential \f$E_{rev}\f$ of the channel [units=V; range=(-1,1); readwrite;]
  float Erev; // this isn analog output

  //! The coductance \f$g(t,V_m)\f$ of the channel [readonly; units=S; range=(0,1);]
  double g;   // this is an analog output

protected:
  friend class IonGate;

  //! Pointer to the membranepatch to which the channel belongs \internal [hidden];
  MembranePatchSimple *membrane;
};

#endif


