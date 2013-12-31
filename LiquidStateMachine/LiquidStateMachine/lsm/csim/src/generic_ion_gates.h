#ifndef __GENERIC_ION_GATES_H_
#define __GENERIC_ION_GATES_H_

#include "viongate.h"

//! A generic voltage dependent ion gate
class GVD_Gate : public VIonGate {

  DO_REGISTERING

public:
  GVD_Gate(void) { Vh=0; Vc=1e-3; Ts=1; Te=1e-3;  C1=0; C2=0;}

  virtual ~GVD_Gate(void) {  if (getC1()) { free(getC1()); setC1(0); }
	                     if (getC2()) { free(getC2()); setC2(0); } }

  virtual double tau(double V) { return Ts*exp( -Te*(V - *Vresting) )/(1 + exp(-((V - *Vresting) - Vh)/Vc)); }

  virtual double infty(double V)  { return 1/(1 + exp(-((V - *Vresting) - Vh)/Vc)); }

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,V)=p_\infty(V)\f$.
  virtual int updateInternal(void);

  //! Inflection point of the sigmoidal function \f$p(V)\f$ [units=Volt; range=(-1,1); readwrite;]
  double Vh; // this is analog output

  //! Determines the slope of the sigmoidal function \f$p(V)\f$ [units=Volt; range=(-1,1); readwrite;]
  double Vc; // this is analog output

  //! Scale of the time constant \f$\tau(V)\f$ [range=(0,1e15); readwrite;]
  double Ts; // this is analog output

  //! Additional exponential factor of the time constant \f$\tau(V)\f$ [range=(-1,1); readwrite;]
  double Te; // this is analog output

protected:
  virtual double *getC1(void) { return C1; }
  virtual double *getC2(void) { return C2; }
  virtual void setC1(double *p) { C1=p; }
  virtual void setC2(double *p) { C2=p; }

  //! The look up table for the exponential Euler integration 'constant' \f$C_1(V)=\exp(-\Delta t / \tau(V))\f$   \internal [hidden]
  double *C1;

  //! The look up table for the exponential Euler integration 'constant' \f$C_2(V)=(1-C_1(V))\cdot p_\infty(V)\f$  \internal [hidden]
  double *C2;
};

//! A generic voltage dependent ion gate with constant time constant
class GVD_cT_Gate : public VIonGate {

  DO_REGISTERING

public:
  GVD_cT_Gate(void) { Vh=0; Vc=1e-3; Ts=1;  C1=0; C2=0; }

  virtual ~GVD_cT_Gate(void) {  if (getC1()) { free(getC1()); setC1(0); }
	                        if (getC2()) { free(getC2()); setC2(0); } }

  virtual double infty(double V)  { return 1/(1 + exp(-((V - *Vresting) - Vh)/Vc)); }

  virtual double tau(double ) { return Ts; }

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,V)=p_\infty(V)\f$.
  virtual int updateInternal(void);

  //! Inflection point of the sigmoidal function \f$p(V)\f$ [units=Volt; range=(-1,1); readwrite;]
  double Vh; // this is analog output

  //! Slope of the sigmoidal function \f$p(V)\f$ [units=Volt; range=(-1,1); readwrite;]
  double Vc; // this is analog output

  //! Voltage independent time constant \f$\tau\f$ [range=(-1,1); readwrite;]
  double Ts; // this is analog output

protected:
  virtual double *getC1(void) { return C1; }
  virtual double *getC2(void) { return C2; }
  virtual void setC1(double *p) { C1=p; }
  virtual void setC2(double *p) { C2=p; }

  //! The look up table for the exponential Euler integration 'constant' \f$C_1(V)=\exp(-\Delta t / \tau(V))\f$   \internal [hidden]
  double *C1;

  //! The look up table for the exponential Euler integration 'constant' \f$C_2(V)=(1-C_1(V))\cdot p_\infty(V)\f$  \internal [hidden]
  double *C2;
};


#endif

