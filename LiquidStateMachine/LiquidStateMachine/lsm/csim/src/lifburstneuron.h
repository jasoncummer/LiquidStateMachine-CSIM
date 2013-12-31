/*! \file lifburstneuron.h
**  \brief Class definition of LifBurstNeuron
*/

#ifndef _LIFBURSTNEURON_H_
#define _LIFBURSTNEURON_H_

#include "spikingneuron.h"

//! A nonstandart leaky-integrate-and-fire (I&F) neuron.
/** that produces bursts dependent on two additional variables
 ** analog to the implementation of dynamical synapses:
 ** see Kaske&Bertschinger "Traveling wave patterns..."
 ** \latexonly  \subsubsection*{Model} \endlatexonly
 ** \htmlonly  <h3>Model</h3> \endhtmlonly
 ** 
 ** A nonstandard leaky-integrate-and-fire neuron model is implemented
 ** where the membrane potential \f$V_m\f$ of a neuron is given by 
 ** \f[
 **   \tau_m \frac{d V_m}{dt} = -(V_m-V_{resting}) + R_m \cdot (I_{syn}(t)+I_{inject}+I_{noise})
 ** \f] 
 ** where \f$\tau_m=C_m\cdot R_m\f$ is the membrane time constant,
 ** \f$R_m\f$ is the membrane resistance, \f$I_{syn}(t)\f$ is the
 ** current supplied by the synapses, \f$I_{inject}\f$ is a
 ** non-specific background current and \f$I_{noise}\f$ is a
 ** Gaussion random variable with zero mean and a given variance
 ** noise. 
 ** Vreset is modulated by u and r to produce spiking dependent on the
 ** previous spike pattern 
 ** At time \f$t=0\f$ \f$V_m\f$ ist set to\f$V_{init}\f$. If 
 ** \f$V_m\f$ exceeds the threshold voltage\f$V_{thresh}\f$ it is 
 ** reset to \f$V_{reset}\f$ and hold there for the length 
 ** \f$T_{refract}\f$ of the absolute refractory period.
 **
 ** \latexonly  \subsubsection*{Implementation} \endlatexonly
 ** \htmlonly  <h3>Implementation</h3> \endhtmlonly
 **
 ** The exponential Euler method is used for numerical integration.
 ** 
 ** */
class LifBurstNeuron : public SpikingNeuron {

 DO_REGISTERING  

 public:

  LifBurstNeuron(void);
  virtual ~LifBurstNeuron();

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

  //! The default voltage to reset \f$V_m\f$ to after a spike. [units=V;range=(-1,1);]   
  float Vreset;

  //! spike pattern dependent treshold \f$V_m\f$ to after a spike. [units=V;range=(-1,1);]  
  float VBthresh;

  //! The initial condition for \f$V_m\f$ at time \f$t=0\f$. [units=V; range=(-1,1);]
  float Vinit;

  //! The length of the absolute refractory period. [units=sec; range=(0,1);]
  float Trefract;

  //! The standard deviation of the noise to be added each integration time constant. [range=(0,1); units=A;]
  float Inoise;

  //! A constant current to be injected into the LIF neuron. [units=A; range=(-1,1);]
  float Iinject;

  //! internal parameter uB - recovery from spike facilitation with time constant FB 
 float uB;

 //! internal parameter rB - recovery from spike depression with time constant DB
 float rB;

 //! FB time constant for recovery from facilitation of spike generation
 float FB;

  //! DB time constant for recovery from depression of spike generation
  float DB;

  //! UB constant for onset of facilitation of spike generation
  float UB;
 
protected:
  //! The membrane voltage \f$V_m\f$ [readonly; units=V;]
  double Vm;
    
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

  //! uB0, rBO updated during spike,control facilitation and depression 
  float uB0;
  float rB0;

//! C3, C4 internal constants for exponential Euler integration of uB and rB
  double C3,C4;
  

};

#endif
