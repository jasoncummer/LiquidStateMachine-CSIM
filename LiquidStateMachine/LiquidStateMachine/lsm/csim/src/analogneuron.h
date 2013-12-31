/*! \file analogneuron.h
**  \brief Class definition of AnalogNeuron
*/

#ifndef _ANALOGNEURON_H_
#define _ANALOGNEURON_H_

#include "neuron.h"
#include "analogsynapse.h"
#include "globaldefinitions.h"

//! Base class for all analog neurons
 /** 
  ** The delays of post-neuron synapses are implemented via a single
  ** queue (class Queue) in the neuron itself to avoid storing the
  ** same data several times. A class derived from AnalogNeuron must
  ** store its membrane potential which is usually calculated during
  ** nextstate() in the variable #Vm.  The method output() of the
  ** neuron calls putInQueue() and then getFromQueue() for each of
  ** its post-neur synapses with the correct delay \c d of the
  ** corresponding synapse.  */
class AnalogNeuron : public Neuron {
 public:
  AnalogNeuron(void);
  ~AnalogNeuron();

  //! The current output (potential) of this neuron [readonly; units=V;]
  /** It is calculated during advance(), possibly overwritten in
   ** force(), and put into the queue during output(). */
  double Vm;

  //! The resting membrane voltage. [units=V; range=(-1,1);]
  float Vresting;

  //! The noise of analog neurons [readwrite; units=A^2;]
  double Inoise;
  virtual double nextstate(void) { return (VmOut = Vm = Vresting); }
  virtual void force(double);
  virtual void reset(void);
  virtual void output(void);
  virtual int addIncoming(Advancable *a);
  virtual int addOutgoing(Advancable *a);

 protected:
  //! Queue for delaying the output of analog synapses
  struct Queue {
    //! Pointer to an array of doubles allocated by malloc()
    double *entry;
    //! Index to the next empty field. This is where new values are being written to
    unsigned int current;
    //! Number of elements in the queue
    unsigned int size;
  };
  //! The vlaue wich will actualle be propagated to the outgoing synapses.
  /**
   ** The implementation of nextstate() should always set
   ** VmOut=Vm. However some Teacher may overwrite this by callinf
   ** force().  */
  double VmOut;

 private:
  //! Queue to hold previous values of Vm; used to implement delays.
  Queue *delayQueue;

  //! Get previously stored value of the membrane voltage from #delayQueue
  double getFromQueue(int delayIndex);

  //! Put membrane voltage into #delayQueue
  void putInQueue(double theValue);

};

#endif
