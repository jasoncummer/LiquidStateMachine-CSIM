#ifndef __HH_SQUID_CHANNELS_H_
#define __HH_SQUID_CHANNELS_H_

#include "viongate.h"
#include "activechannel.h"

#define VSCALE 1000
#define TSCALE 1000 

//! n gate for the fast HH potassium (K) channel
/**
 ** See HH_K_Channel
 **
 */
class HH_n_Gate : public VIonGate {
public:
  HH_n_Gate(void) { k=4; }
  double alpha(double V) { return TSCALE*(fabs(10-(V-*Vresting)*VSCALE) < 1e-15 ? 
					  0.1 : 0.01*(10-(V-*Vresting)*VSCALE)/(exp((10-(V-*Vresting)*VSCALE)/10)-1.0)); }
  double beta(double V)  { return TSCALE*(0.125*exp(-(V-*Vresting)*VSCALE/80)); }  
  IONGATE_TABLES(HH_n_Gate);
};


//! Hodgkin and Huxley fast potassium (K) channel for AP generation.
/**
 ** Uses the gate HH_n_Gate
 **
 */
class HH_K_Channel : public ActiveChannel {

  DO_REGISTERING

public:

  HH_K_Channel(double Gbar=0.2827430964*1e-3,double Erev=-11.99979305*1e-3) {
    addGate(new HH_n_Gate);
    this->Gbar = Gbar;
    this->Erev = Erev;
  }

  virtual ~HH_K_Channel(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

};

//! m gate for the fast HH sodium (Na) channel
/**
 ** see HH_Na_Channel
 **
 */
class HH_m_Gate : public VIonGate {
public:
  HH_m_Gate(void) { k=3; }
  double alpha(double V) { return TSCALE*(fabs(25-(V-*Vresting)*VSCALE)<1e-15 ?
					  1 : (0.1*(25-(V-*Vresting)*VSCALE))/(exp((25-(V-*Vresting)*VSCALE)/10)-1.0)); }
  double beta(double V)  { return TSCALE*(4.0*exp(-(V-*Vresting)*VSCALE/18)); }
  IONGATE_TABLES(HH_m_Gate);
};

//! h gate for the fast HH sodium Na channel
/**
 ** see HH_Na_Channel
 **
 */
class HH_h_Gate : public VIonGate {
public:
  HH_h_Gate(void) { k=1; }
  double alpha(double V) { return TSCALE*(0.07*exp(-(V-*Vresting)*VSCALE/20)); }
  double beta(double V)  { return TSCALE*(1.0/(exp((30-(V-*Vresting)*VSCALE)/10)+1)); }
  IONGATE_TABLES(HH_h_Gate);
};

//! Hodgkin and Huxley fast sodium (Na) channel for AP generation.
/**
 ** Uses the gates HH_h_Gate and HH_m_Gate
 **
 */
class HH_Na_Channel : public ActiveChannel {

  DO_REGISTERING

public:

  HH_Na_Channel(double Gbar=0.9424769878*1e-3,double Erev=0.1150009537) {
    addGate(new HH_m_Gate);
    addGate(new HH_h_Gate);
    this->Gbar = Gbar;
    this->Erev = Erev;
  }

  virtual ~HH_Na_Channel(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

};

#endif



