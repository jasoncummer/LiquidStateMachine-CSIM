#ifndef __VIONGATE_H__
#define __VIONGATE_H__

#include <math.h>
#include <stdlib.h>
#include "advanceable.h"
#include "membranepatchsimple.h"
#include "iongate.h"

//! The lowest value of \f$V\f$ for which the the lookup tables for \f$C_1(V), C_2(V)\f$ are defined [units=Volt].
#define VIONGATE_VM_MIN -0.100     // -100mV

//! The largest value of \f$V\f$ for which the the lookup tables for \f$C_1(V), C_2(V)\f$ are defined [units=Volt].
#define VIONGATE_VM_MAX +0.100     // +100mV

//! The resolution of \f$V\f$ for the lookup tables for \f$C_1(V), C_2(V)\f$ [units=Volt].
#define VIONGATE_VM_INC +0.00002   // 0.05mV

#define VIONGATE_TABLE_SIZE      ((int)((VIONGATE_VM_MAX - VIONGATE_VM_MIN) / VIONGATE_VM_INC + 1))


//! Generic first order kinitics voltage dependend ion gate.
/**
 ** Generic first order kinitics voltage dependend ion gate.
 **
 ** \model
 **
 ** An ion gate is model via the equation \f$P(t,V) = p(t,V)^k\f$
 ** (\link #k \f$k\f$ \endlink is an interger exponent) where the
 ** state variable \link #p \f$p(t,V)\f$ \endlink obeys the first
 ** order kinetics
 **
 ** \f[
 **    \frac{\delta p(t,V)}{\delta t} = \alpha(V)(1-p(t,V)) - \beta(V)p(t,V)
 ** \f]
 ** 
 ** with the voltage dependent \e rate \e constants \link #alpha
 ** \f$\alpha(V)\f$ \endlink and \link #beta \f$\beta(V)\f$
 ** \endlink. This equation can also be written in the (sometimes more
 ** convinient) form
 **
 ** \f[
 **    \tau(V)\frac{\delta p(t,V)}{\delta t} = -p(t,V) + p_\infty(V)
 ** \f]
 **
 ** with the following relations between the time ``constant'' \link
 ** #tau \f$\tau(V)\f$ \endlink and the ``resting'' value \link #infty
 ** \f$p_\infty(V)\f$ \endlink and the activation and inactivation
 ** parameters \f$\alpha(V)\f$ and \f$beta(V)\f$.
 **
 ** \f[ 
 **   \tau(V) = \frac{1}{\alpha(V)+\beta(V)} \text{ and } p_\infty(V) = \frac{\alpha(V)}{(\alpha(V)+\beta(V))}
 ** \f]
 ** \f[
 **   \alpha(V) = \frac{p_\infty(V)}{\tau(V)}  \text{ and } \beta(v) = \frac{1-p_\infty(V)}{\tau(V)}
 ** \f]
 **
 ** \endmodel
 **
 ** \implementation
 **
 ** For an efficient computation we precalculate for each value of
 ** \f$V\f$ the integration parameters \f$C_1(V)\f$ and \f$C_2(V)\f$
 ** for the numerical solution of the differential equation and store
 ** them in a lookup table.
 **
 ** For the exponential Euler integration method these are given by
 ** \f[C_1(V)=\exp(-\Delta t / \tau(V))\f] and
 ** \f[C_2(V)=(1-C_1(V))\cdot p_\infty(V)\f].
 **
 ** For the Crank-Nicolson method they are given by
 ** \f[C_1(V)= \frac{1 - \Delta t/2*(\alpha(v) + \beta(v))}{1 + \Delta t/2*(\alpha(v) + \beta(v))}\f] and
 ** \f[C_2(V)= \frac{\Delta t*\alpha(v)}{1+\Delta t/2*(\alpha(v) + \beta(v))}\f].
 **
 ** \endimplementation
 **
 ** \inout
 **
 ** The calculated output of IonGate is \link #P \f$P(t,V)\f$
 ** \endlink.  For its computations it needs the membrane voltage and
 ** resting potential of the associated object.
 **
 ** \endinout
 **
 ** <b>Implementig a specific IonGate</b>
 **
 ** To actually implement a specific gate one has to derive a class
 ** from this one and implement either the pair of functions alpha(),
 ** beta() or the pair tau(), infty(). Not dooing so will cause an
 ** infity recursion fo the current implementation of IonGate.
 **
 ** Furthermore it is neccesarry that you include in your class
 ** definition of \c MyIonGate the macro \c IONGATE_TABLES and you
 ** also need in \c mygate.cpp the following two lines of code:
 ** \code
 **   double *MyIonGate::C1=0;
 **   double *MyIonGate::C2=0;
 ** \endcode
 ** which define your tables for \f$C_1(V)\f$ and \f$C_2(V)\f$.
 **
 ** */
class VIonGate : public IonGate {

public:
  //! The constructor ..
  VIonGate(void) { Vm=0; Vresting=0; VmScale=0; nummethod=0; dttable=0;};

  // NOTE: The destructor comes within the macro IONGATE_TABLES

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,V)=p_\infty(V)\f$.
  virtual void reset(void);

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,V)=p_\infty(V)\f$.
  virtual int updateInternal(void);

  //! Advance one time step using exponential Euler integration.
  virtual int advance(void);

  //! The activation parameter \f$\alpha(V) \in [0,1]\f$
  virtual double alpha(double V) { return (infty(V))/tau(V); }

  //! The inactivation parameter \f$\beta(V) \in [0,1]\f$
  virtual double beta(double V) { return (1.0-infty(V))/tau(V); }

  //! The "time constant"  \f$\tau(V)\f$
  virtual double tau(double V) { return  1.0/(beta(V)+alpha(V)); }

  //! The "resting value"  \f$p_\infty(V)\f$
  virtual double infty(double V) { return alpha(V)/(beta(V)+alpha(V)); }

  virtual double pInfty(MembranePatchSimple *m) { return infty(m->Vm);}

  //! The gate needs to know the membrane voltage which we set with this function
  virtual void ConnectToMembrane(MembranePatchSimple *m) { this->Vm = &(m->Vm); this->Vresting = &(m->Vresting); this->VmScale = &(m->VmScale);}

  //! Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1[units=flag; range=(0,1);];
  int nummethod;

protected:

  IONGATE_TABLES(VIonGate);

  //! A pointer to the relevant resting membrane potential. \internal [hidden]
  float  *Vresting;

  //! A pointer to the relevant membrane potential.  \internal [hidden]
  double *Vm;

  //! A pointer to the parameter that defines the difference between Vresting and the Vthresh for the calculation of the iongate tables.  \internal [hidden]
  float *VmScale;

  //! Store integration time step for lookup table generation  \internal [hidden]
  double dttable;
};

#endif



