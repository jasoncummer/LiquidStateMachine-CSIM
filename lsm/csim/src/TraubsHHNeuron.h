#ifndef TraubsHHNeuron_H_
#define TraubsHHNeuron_H_

#include "cbneuron.h"
#include "viongate.h"
#include "activechannel.h"

class Traubs_HH_n_Gate;
class Traubs_HH_K_Channel;

class Traubs_HH_h_Gate;
class Traubs_HH_m_Gate;
class Traubs_HH_Na_Channel;

//! Conductance based spiking neuron using Traubs modified HH model.
/** 
 * This model is a modified version of Traub's model of Na/K currents
 * for action potentials in hippocampal pyramidal cells, which was
 * modified from HH equations to fit the kinetics of those neurons; the
 * rate constants correspond to a temperature of 36 deg C
 * 
 * The model is based on a CbNeuron and includes the Traubs_HH_K_Channel and
 * Traubs_HH_Na_Channel for action potetial generation.
 * 
 * INa = gNa * m^3 * h * (v-ENa)
 * IKd = gKd * n^4 * (v-EK)
 * 
 * m' = (m_inf - m) / tau_m
 * h' = (h_inf - h) / tau_h
 * n' = (n_inf - n) / tau_n
 * 
 * v2 = v - Vtr
 *
 * alpha_m = 0.32 * (13-v2) / ( exp((13-v2)/4) - 1)
 * beta_m = 0.28 * (v2-40) / ( exp((v2-40)/5) - 1)
 * tau_m = 1 / (alpha_m + beta_m)
 * m_inf = alpha_m / (alpha_m + beta_m)
 *
 * alpha_h = 0.128 * exp((17-v2)/18)
 * beta_h = 4 / ( 1 + exp((40-v2)/5) )
 * tau_h = 1 / (alpha_h + beta_h)
 * h_inf = alpha_h / (alpha_h + beta_h)
 *
 * alpha_n = 0.032 * (15-v2) / ( exp((15-v2)/5) - 1)
 * beta_n = 0.5 * exp((10-v2)/40)
 * tau_n = 1 / (alpha_n + beta_n)
 * n_inf = alpha_n / (alpha_n + beta_n)
 *
 * HH parameters:
 *
 * gNa = 0.1 S/cm2
 * gKd = 0.03 S/cm2
 * ENa = 50 mV
 * EK = -90 mV
 * Vtr = -63 mV (adjusts threshold to around -50 mV)
 * 
 */
class TraubsHHNeuron : public CbNeuron {

 // This tells CSIM that you want to be able to access this object from Matlab
 DO_REGISTERING

public:
  //! Constructor
  TraubsHHNeuron(void);
  //! Destructor
  virtual ~TraubsHHNeuron();
  //! Within this function the model has to be built
  virtual int init(Advancable *a);
  //! This function has to call updateInternal of the channels
  virtual int updateInternal(void);

private:
  //! The K channel
  Traubs_HH_K_Channel  *k;
  //! The Na channel
  Traubs_HH_Na_Channel *na;
  //! This parameter adjusts the threshold
  float Vtr;

};

//! Factor to convert from volts to millivolts.
/** The voltage dependent activation variables alpha(V) and beta(V) are expressed 
 *  for millivolts and milliseconds but we will use Volt and seconds as units.
 *  Hence in each equation the voltages are multiplied by VSCALE and 
 *  the the results are multiplied by TSCALE.
 */
#define VSCALE 1000

//! Factor to convert from 1/ms to 1/sec
#define TSCALE 1000

/////////////////////////////////////////////////////////////////////////
//                       Traubs_HH_K_Channel  
/////////////////////////////////////////////////////////////////////////

//! n gate for Traub's modified fast HH potassium (K) channel
/**
 * See Traubs_HH_K_Channel
 *
 * alpha_n = 0.032 * (15-v2) / ( exp((15-v2)/5) - 1)
 * beta_n = 0.5 * exp((10-v2)/40)
 * 
 */
class Traubs_HH_n_Gate : public VIonGate {

public:

  Traubs_HH_n_Gate(float Vtr) { 
    k=4; this->Vtr=Vtr;
  }

  double alpha(double V) { 
    return TSCALE*( fabs(15-(V-Vtr)*VSCALE) < 1e-15 ?
                     0.032 * 5 : 0.032*(15-(V-Vtr)*VSCALE)/(exp((15-(V-Vtr)*VSCALE)/5.0)-1.0) ); 
  }

  double beta(double V)  { 
    return TSCALE*(0.5*exp((10-(V-Vtr)*VSCALE)/40.0)); 
  }

  // This macro must be called for each new type of gate
  IONGATE_TABLES(Traubs_HH_n_Gate);

private:
  float Vtr;

};

//! Traubs modified version of Hodgkin and Huxley's fast potassium (K) channel for AP generation.
/** Used in the TraubsHHNeuron neuron model.
 *
 *  Uses the gate Traubs_HH_n_Gate
 *
 * gKd = 0.03 S/cm2
 * EK = -90 mV
 */
class Traubs_HH_K_Channel : public ActiveChannel {

  DO_REGISTERING

public:

  Traubs_HH_K_Channel(double Gbar=0.06*1e-4, double Erev=-0.09, float Vtr = -0.063) {
    addGate(new Traubs_HH_n_Gate(Vtr));
    this->Gbar = Gbar;
    this->Erev = Erev;
  }

  virtual ~Traubs_HH_K_Channel(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

};


/////////////////////////////////////////////////////////////////////////
//                       Traubs_HH_Na_Channel  
/////////////////////////////////////////////////////////////////////////


//! m gate for the fast Traubs modified HH sodium (Na) channel
/** 
 * Used in TraubsHHNeuron neuron model.
 * 
 * see Traubs_HH_Na_Channel
 * 
 * alpha_m = 0.32 * (13-v2) / ( exp((13-v2)/4) - 1)
 * beta_m = 0.28 * (v2-40) / ( exp((v2-40)/5) - 1)
 * v2 = v - v_resting (v2 is in mV)
 */
class Traubs_HH_m_Gate : public VIonGate {

public:

  Traubs_HH_m_Gate(float Vtr) { k=3; this->Vtr = Vtr; }

  double alpha(double V) { return TSCALE*( fabs(13-(V-Vtr)*VSCALE)<1e-15 ?
					 4*0.32 : (0.32*(13-(V-Vtr)*VSCALE))/(exp((13-(V-Vtr)*VSCALE)/4.0)-1.0)) ; }

  double beta(double V)  { return TSCALE*( fabs(40-(V-Vtr)*VSCALE)<1e-15 ?
					 0.28 * 5 : (0.28*((V-Vtr)*VSCALE-40))/(exp(((V-Vtr)*VSCALE-40)/5.0)-1.0)); }

  IONGATE_TABLES(Traubs_HH_m_Gate);

private:  

  float Vtr;

};

//! h gate for the fast Traubs modified HH sodium Na channel
/**
 * Used in the TraubsHHNeuron neuron model.
 * 
 * see Traubs_HH_Na_Channel.
 *
 * alpha_h = 0.128 * exp((17-v2)/18)
 * beta_h = 4 / ( 1 + exp((40-v2)/5) ) 
 */
class Traubs_HH_h_Gate : public VIonGate {

public:
  Traubs_HH_h_Gate(float Vtr) { k=1; this->Vtr = Vtr; }

  double alpha(double V) { return TSCALE*(0.128*exp((17-(V-Vtr)*VSCALE)/18.0)); }

  double beta(double V)  { return TSCALE*(4.0/(exp((40-(V-Vtr)*VSCALE)/5.0)+1)); }

  IONGATE_TABLES(Traubs_HH_h_Gate);

private: 
  float Vtr;

};

//! Traubs modified version of Hodgkin and Huxley fast sodium (Na) channel for AP generation.
/**
 * Used in the TraubsHHNeuron neuron model.
 * 
 * Uses the gates Traubs_HH_h_Gate and Traubs_HH_m_Gate.
 *
 * gNa = 0.1 S/cm2
 * ENa = 50 mV
 */
class Traubs_HH_Na_Channel : public ActiveChannel {

  DO_REGISTERING

public:

  Traubs_HH_Na_Channel(double Gbar=0.2*1e-4,double Erev=0.050, float Vtr = -0.063) {
    addGate(new Traubs_HH_m_Gate(Vtr));
    addGate(new Traubs_HH_h_Gate(Vtr));
    this->Gbar = Gbar;
    this->Erev = Erev;
  }

  virtual ~Traubs_HH_Na_Channel(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

};

#endif /*TraubsHHNeuron_H_*/
