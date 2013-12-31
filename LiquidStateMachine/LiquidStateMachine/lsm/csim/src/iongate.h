#ifndef __IONGATE_H__
#define __IONGATE_H__

#include <math.h>
#include <stdlib.h>
#include "advanceable.h"
#include "membranepatchsimple.h"

#define IONGATE_TABLES(_className_) /* this must be included by each class derived from IonGate */ \
protected: \
 /** virtual methods to correctly acces the static members C1 and C2 */ \
 virtual double *getC1(void) { return C1; } \
 virtual double *getC2(void) { return C2; } \
 virtual void setC1(double *p) { C1=p; } \
 virtual void setC2(double *p) { C2=p; } \
 /** The look up table for the exponential Euler integration 'constant' \f$C_1(V)=\exp(-\Delta t / \tau(V))\f$ */ \
 static   double *C1; \
 /** The look up table for the exponential Euler integration 'constant' \f$C_2(V)=(1-C_1(V))\cdot p_\infty(V)\f$ */ \
 static   double *C2; \
 /** unfortunately we also have to include the destructor since apparently */ \
 /** (at least with gcc 2.95) the virtuals are not called correctly within the destructor */ \
public: \
 ~_className_(void) { \
   if (getC1()) { free(getC1()); setC1(0); } \
   if (getC2()) { free(getC2()); setC2(0); } \
 }

//! Generic first order kinetics ion gate template.
/*
 ** Missing
 **
 ** */
class IonGate : public Advancable {

public:
  //! The constructor ..
  IonGate(void) { k=1;  P=0;  c1=0;  c2=0;  p=0; };

  // NOTE: The destructor comes within the macro IONGATE_TABLES

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,V)=p_\infty(V)\f$.
  virtual void reset(void) {};

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,V)=p_\infty(V)\f$.
  virtual int updateInternal(void) {return 0;};

  //! Advance one time step using exponential Euler integration.
  virtual int advance(void) {return 0;};

  virtual double pInfty(MembranePatchSimple *) { return 0;}

  virtual int addIncoming(Advancable *a);

  virtual int addOutgoing(Advancable *a);

  //! The gate needs to know the membrane voltage which we set with this function
  virtual void ConnectToMembrane(MembranePatchSimple *) { return;}

  //! The exponent of the gate [readwrite; units=1; range=(1,100);]
  int k;

  //! The output \f$P(t,V) = p(t,V)^k\f$ of the gate. [readonly; units=1; range=(0,1);]
  double P;

protected:

  IONGATE_TABLES(IonGate);

  //! The pointer to the lookuptable for \f$C_1(V_m)\infty\f$ \internal [hidden]
  double *c1;
  //! The pointer to the lookuptable for \f$C_2(V_m)\f$ \internal [hidden]
  double *c2;

  //! The state variable.
  double p;
};

#endif



