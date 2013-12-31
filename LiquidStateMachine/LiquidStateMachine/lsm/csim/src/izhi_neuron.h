/*! \file izhi_neuron.h
**  \brief Class definition of IZHI_Neuron
*/

#ifndef _IZHI_NEURON_H_
#define _IZHI_NEURON_H_

#include "spikingneuron.h"

//! A canonical bursting and spiking neuron.
/** see Izhikevich E. "Simple model of Spiking Neurons"
 ** \latexonly  \subsubsection*{Model} \endlatexonly
 ** \htmlonly  <h3>Model</h3> \endhtmlonly
 ** 
 ** A canonical neuron model is implemented
 ** where the membrane potential \f$V_m\f$ of a neuron is given by 
 ** 
 **   
 **  
 ** ,
 ** \f$R_m\f$ is the membrane resistance, \f$I_{syn}(t)\f$ is the
 ** current supplied by the synapses, \f$I_{inject}\f$ is a
 ** non-specific background current and \f$I_{noise}\f$ is a
 ** Gaussion random variable with zero mean and a given variance
 ** noise. 
 **
 ** At time \f$t=0\f$ \f$V_m\f$ ist set to \f$V_{init}\f$. If
 ** \f$V_m\f$ exceeds the threshold voltage \f$V_{thresh}\f$ it is
 ** reset to \f$V_{reset}\f$ and hold there for the length
 ** \f$T_{refract}\f$ of the absolute refractory period.
 **
 ** \latexonly  \subsubsection*{Implementation} \endlatexonly
 ** \htmlonly  <h3>Implementation</h3> \endhtmlonly
 **
 ** The simple Euler method is used for numerical integration.
 ** 
 ** */
class Izhi_Neuron : public SpikingNeuron {

 DO_REGISTERING  

 public:

  Izhi_Neuron(void);
  virtual ~Izhi_Neuron();

  //! Recalculate exp. Euler constants
  virtual int updateInternal(void);

  //! Reset the leaky I&F neuron.
  virtual void reset(void);

  //! Calculate the new membrane voltage and check wheter Vm exceeds the threshold V_thresh.
  virtual double nextstate(void);

  //! Returns 1 (0) if the neuron is (not) in its absolute refractory period.
  virtual int isRefractory(void);

  //! The membrane capacity \f$C_m\f$ [range=(0,1); units=F;]
  float Cm;

  //! The membrane resistance \f$R_m\f$ [units=Ohm; range=(0,1e30)]
  float Rm;

  //! If \f$V_m\f$ exceeds \f$V_{thresh}\f$ a spike is emmited. [units=V; range=(-10,100);]
  float Vthresh;

  //! The resting membrane voltage. [units=V; range=(-1,1);]
  float Vresting;

  //! The voltage to reset \f$V_m\f$ to after a spike. [units=V; range=(-1,1);]
  float Vreset;

  //! The initial condition for \f$V_m\f$ at time \f$t=0\f$. [units=V; range=(-1,1);]
  float Vinit;

  //! The length of the absolute refractory period. [units=sec; range=(0,1);]
  float Trefract;

  //! The standard deviation of the noise to be added each integration time constant. [range=(0,1); units=A;]
  float Inoise;

  //! A constant current to be injected into the LIF neuron. [units=A; range=(-1,1);]
  float Iinject;

  //! A constant (0.02, 01) describing the coupling of variable u to Vm;    
  float a;  

  //! A constant controlling sensitivity og u
  float b;

  //! A constant controlling reset of Vm (1000*Vreset)
  float c;

  //! A constant controlling reset of u 
  float d;


protected:
  //! The membrane voltage \f$V_m\f$ [readonly; units=V;]
  double Vm;

//! The membrane voltage in millivolt
  double Vb;

//! The membrane voltage in millivolt
  double Vint;

//! internal variable
  double u;

//! The internal variable u adapted to millivolt and millisecond
  double ub;
    
  //! synaptic input current
  float Isyn;

  // protected:
  // The total current: noise + inject +
  //double Itot;
//int nStepsInRefr;

 private:
  //! If positive then this counter tells us how many time steps we are still in the absolute refractory period
  int nStepsInRefr;

  //! Internal constants for the exponential Euler integration of Vm.
  double C1,C2,I0;

};

#endif
