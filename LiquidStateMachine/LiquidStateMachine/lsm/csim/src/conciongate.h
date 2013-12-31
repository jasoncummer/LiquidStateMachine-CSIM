#ifndef __CONCIONGATE_H__
#define __CONCIONGATE_H__

#include <math.h>
#include <stdlib.h>
#include "advanceable.h"
#include "membranepatch.h"
#include "iongate.h"


//! The lowest value of \f$V\f$ for which the the lookup tables for \f$C_1(V), C_2(V)\f$ are defined [units=Volt].
#define CONCIONGATE_CONC_MIN 0.00      // [Mol]

//! The largest value of \f$V\f$ for which the the lookup tables for \f$C_1(V), C_2(V)\f$ are defined [units=Volt].
#define CONCIONGATE_CONC_MAX 1000e-9   // [Mol]

//! The resolution of \f$V\f$ for the lookup tables for \f$C_1(V), C_2(V)\f$ [units=Volt].
#define CONCIONGATE_CONC_INC 5.0e-10   // 0.25mV

#define CONCIONGATE_TABLE_SIZE      ((int)((CONCIONGATE_CONC_MAX - CONCIONGATE_CONC_MIN) / CONCIONGATE_CONC_INC + 1))


//! Generic first order kinitics ion concentration dependend ion gate.
/**
 ** Generic first order kinitics ion concentration dependend ion gate.
 **
 ** missing ...
 ** */
class ConcIonGate : public IonGate {

public:
  //! The constructor ..
  ConcIonGate(void) { Conc=0; ConcRest=0; ConcScale=0; ConcType=100; };

  // NOTE: The destructor comes within the macro IONGATE_TABLES

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,Conc)=p_\infty(Conc)\f$.
  virtual void reset(void);

  //! Set the initial conditions for time \f$t=0\f$: \f$p(0,Conc)=p_\infty(Conc)\f$.
  virtual int updateInternal(void);

  //! Advance one time step using exponential Euler integration.
  virtual int advance(void);

  //! The "time constant"  \f$\tau(Conc)\f$
  virtual double tau(double ) { return  0; }

  //! The "resting value"  \f$p_\infty(Conc)\f$
  virtual double infty(double ) { return 0; }

  virtual double pInfty(MembranePatchSimple *m);

  //! The gate needs to know the ion concentration parameters which we set with this function
  virtual void ConnectToMembrane(MembranePatchSimple *m);

protected:

  IONGATE_TABLES(ConcIonGate);

  //! A pointer to the relevant resting membrane potential. \internal [hidden]
  float  *ConcRest;

  //! A pointer to the relevant membrane potential.  \internal [hidden]
  double *Conc;

  //! A pointer to the parameter that defines the difference between Vresting and the Vthresh for the calculation of the iongate tables.  \internal [hidden]
  float  *ConcScale;

  //! A pointer to the parameter that defines the difference between Vresting and the Vthresh for the calculation of the iongate tables.  \internal [hidden]
  int  ConcType;

};

#endif



