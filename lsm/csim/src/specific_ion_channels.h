#ifndef __SPECIFIC_ION_CHANNELS_H_
#define __SPECIFIC_ION_CHANNELS_H_

#include "viongate.h"
#include "conciongate.h"
#include "activechannel.h"
#include "activecachannel.h"
#include "csimerror.h"

// parameters for voltage range transformations
#define V_REST_BIOL_MV   (-70)    // [mV] (general in publications used unit)
#define V_THRESH_BIOL_MV (-40)    // [mV]
#define V_REST_SIM    (0.0)   // [V]  (all simulation parameters are in SI units)
#define V_THRESH_SIM  (15e-3) // [V]

#define A_MEMBRANE (0.01e-2) // [cm2]

/* Transform biological Erev value to model voltage range */
/* Note that in the ActiveChannel constructor actual Neuron voltages are unknown */ 
#define TRANSFORM_EREV Erev = ((Erev - V_REST_BIOL_MV)*(V_THRESH_SIM - V_REST_SIM)/(V_THRESH_BIOL_MV-V_REST_BIOL_MV) + V_REST_SIM);


#define TRANSFORM_V    /* Transform model voltage value to biological voltage range */ \
                       V = (V - *Vresting)*(V_THRESH_BIOL_MV - V_REST_BIOL_MV)/(*VmScale) + V_REST_BIOL_MV

#define TSCALE 1000
#define VSCALE 1000

//============================================================

#define CASCALE 1.1180e+07

//! The lowest value of \f$[Ca]\f$ for which the the lookup tables for \f$C_1([Ca]), C_2([Ca])\f$ are defined [units=Mol].
#define IONGATE_CA_MIN 0.0

//! The largest value of \f$[Ca]\f$ for which the the lookup tables for \f$C_1([Ca]), C_2([Ca])\f$ are defined [units=Mol].
#define IONGATE_CA_MAX 1000e-9

//! The resolution of \f$[Ca]\f$ for the lookup tables for \f$C_1([Ca]), C_2([Ca])\f$ [units=Mol].
#define IONGATE_CA_INC 5.0e-10

#define IONGATE_CA_TABLE_SIZE      ((int)((IONGATE_CA_MAX - IONGATE_CA_MIN) / IONGATE_CA_INC + 1))

//! Ca concentration dependent ion gate.
/** From: Yamada, Koch and Adams: Multiple Channels and Calcium Dynamics, Editors: Koch, Segev, MIT Press 1998).
  * Can only be connected to CaChannel_Yamada98 class, where the Ca mechanism is included
  */
class CaGate_Yamada98 : public VIonGate {

  DO_REGISTERING

public:
  CaGate_Yamada98(void) { k=2; Ca=0; Ts=1; C1=0; C2=0; }

  virtual ~CaGate_Yamada98(void) { if (getC1()) { free(getC1()); setC1(0); }
   				   if (getC2()) { free(getC2()); setC2(0); }}

  virtual double tau(double C) { return Ts/( pow(CASCALE*C,2) + 2.5); }

  virtual double infty(double C)  { return pow(CASCALE*C,2)/( pow(CASCALE*C,2) + 2.5); }

  virtual double pInfty(MembranePatchSimple *) { return 0;}

  virtual void reset(void);

  virtual void setCa(double *C) { Ca = C; }

  virtual int updateInternal(void);

  virtual int advance(void);

  virtual int addIncoming(Advancable *Incoming);

  //! Scale of the time constant \f$\tau(Ca)\f$ [range=(0,1e15); readwrite;]
  double Ts;

protected:

   //! A pointer to the relevant [Ca] concentration.  \internal [hidden]
  double *Ca;

  virtual double *getC1(void) { return C1; }
  virtual double *getC2(void) { return C2; }
  virtual void setC1(double *p) { C1=p; }
  virtual void setC2(double *p) { C2=p; }

  //! The look up table for the exponential Euler integration 'constant' \f$C_1(V)=\exp(-\Delta t / \tau(V))\f$   \internal [hidden]
  double *C1;

  //! The look up table for the exponential Euler integration 'constant' \f$C_2(V)=(1-C_1(V))\cdot p_\infty(V)\f$  \internal [hidden]
  double *C2;
};

//== Channels used by James Maciokas / Reno ==========================================================

//! Ca concentration dependent ion channel
/** From: Yamada, Koch and Adams: Multiple Channels and Calcium Dynamics, Editors: Koch, Segev, MIT Press 1998)
  * Ca mechanism is included in the channel class not the neuron class.
  */
class CaChannel_Yamada98 : public ActiveChannel {

  DO_REGISTERING

public:

  CaChannel_Yamada98(void) {
    Erev = -5e-6 * VSCALE; // assumed resting potential Vresting = 0
    u = 200e-9;   // Mol
    Ts = 70e-3;   // Sec
    Ca = 0;       // Mol
    Gbar = 54e-9; // S
  }

  virtual void membraneSpikeNotify(double ) { Ca = Ca + u; };

  virtual void reset(void);

  virtual int updateInternal(void);

  virtual int advance(void);

  //! Constant step increase in the calcium concentration [Ca] for each spike; [units=Mol; range=(0,1); readwrite;]
  float u;

  //! Time constant for the deactivation of the calcium concentration [Ca];  [units=Sec; range=(0,1e15); readwrite;]
  float Ts;

  //! Interior calcium concentration [Ca]; [units=Mol; range=(0,1); readwrite;]
  double Ca;

protected:

  //! Constant for the exponential Euler integration step; \internal [hidden]
  float C1;
};

//! Voltage dependent ion gate from Wang et al., 1998
/** From:  Wang, H.-S., Pan, Z., Shi, W., Brown, B. S., Wymore, R.S., Cohen, I. S., Dixon, J. E. and McKinnon, D. (1998) KCNQ2 and KCNQ3Potassium Channel Subunits: Molecular Correlates of the M-Channel. Science, 282, 1890-1893
*/
class MmGate_Wang98 : public VIonGate {
public:
  MmGate_Wang98(void) { k=1; }
  double tau(double V) { return TSCALE * exp(+( VSCALE * (V - *Vresting) - 12.3)/9.15) / 3800; }
  double infty(double V)  { return 1/(1 + exp(-( VSCALE * (V - *Vresting) - 13)/4.4)); }

  IONGATE_TABLES(MmGate_Wang98);
};



//! Voltage dependent ion channel from Wang et al., 1998
/** From:  Wang, H.-S., Pan, Z., Shi, W., Brown, B. S., Wymore, R.S., Cohen, I. S., Dixon, J. E. and McKinnon, D. (1998) KCNQ2 and KCNQ3Potassium Channel Subunits: Molecular Correlates of the M-Channel. Science, 282, 1890-1893 */
class MChannel_Wang98 : public ActiveChannel {

  DO_REGISTERING

public:

  MChannel_Wang98(void) {
    addGate(new MmGate_Wang98);

    Erev = -80; // [mV] biological value
    // Transform to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~MChannel_Wang98(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }
};

//! Voltage dependent ion gate from Hoffman et al., 1997
/** Used by AChannel_Hoffman97
  *
  * Reference: Hoffman, D., Magee, J.C., Colbert, C.M., and Johnston, D.
  * Potassium channel regulation of signal propagation in dendrites of hippocampal pyramidal neurons.
  * Nature 387:869-875 (1997)
  */
class AmGate_Hoffman97 : public VIonGate {
public:
  AmGate_Hoffman97(void) { k=1; }


  double tau(double V){ return TSCALE*0.2e-6; }
  double infty(double V) { TRANSFORM_V;
  			   return 1/(1+exp(-(V-11)/18)); }

/* Directly transformed parameters
  double tau(double ) { return TSCALE*0.2e-6; }
  double infty(double V)  { return 1/(1 + exp(-( VSCALE * (V - *Vresting) - 40.5)/9)); }
*/
  IONGATE_TABLES(AmGate_Hoffman97);
};


//! Voltage dependent ion gate from Hoffman et al., 1997
/** Used by AChannel_Hoffman97
  *
  * Reference: Hoffman, D., Magee, J.C., Colbert, C.M., and Johnston, D.
  * Potassium channel regulation of signal propagation in dendrites of hippocampal pyramidal neurons.
  * Nature 387:869-875 (1997)
  */
class AhGate_Hoffman97 : public VIonGate {
public:
  AhGate_Hoffman97(void) { k=1; }

  double tau(double V) { TRANSFORM_V;
  			 return (V < -20) ? 5e-3 : 1e-3 * (5+2.6*(V+20)/10); }
  double infty(double V) { TRANSFORM_V;
  			   return 1/(1+exp((V+56)/8)); }


/* Directly transformed parameters
  double tau(double ) { return TSCALE*5e-6; }
  double infty(double V)  { return 1/(1 + exp(( VSCALE * (V - *Vresting) - 7)/4)); }
*/
  IONGATE_TABLES(AhGate_Hoffman97);
};



//! Voltage dependent ion channel from Hoffman et al., 1997
/** Uses AmGate_Hoffman97 and AhGate_Hoffman97
  *
  * Reference: Hoffman, D., Magee, J.C., Colbert, C.M., and Johnston, D.
  * Potassium channel regulation of signal propagation in dendrites of hippocampal pyramidal neurons.
  * Nature 387:869-875 (1997)
  */
class AChannel_Hoffman97 : public ActiveChannel {

  DO_REGISTERING

public:

  AChannel_Hoffman97(void) {
    addGate(new AmGate_Hoffman97);
    addGate(new AhGate_Hoffman97);

    Erev = -80; // [mV] biological value
    // Transform to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~AChannel_Hoffman97(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }
};

//! Voltage dependent ion gate from Maciokas et al. 2002
/** Used by SICChannel_Maciokas02

 Reference: Maciokas, J., Goodman, P., Kenyon, J., Accurate Dynamical Model of Interneuronal GABAergic Channel Physiologies
*/
class SICmGate_Maciokas02 : public VIonGate {
public:
  SICmGate_Maciokas02(void) { k=1; }

  double tau(double ) { return TSCALE*0.2e-6; }
  double infty(double V)  { return 1/(1 + exp(-( VSCALE * (V - *Vresting) - 20)/2.5)); }

  IONGATE_TABLES(SICmGate_Maciokas02);
};

//! Voltage dependent ion gate from Maciokas et al. 2002
/** Used by SICChannel_Maciokas02

 Reference: Maciokas, J., Goodman, P., Kenyon, J., Accurate Dynamical Model of Interneuronal GABAergic Channel Physiologies
*/
class SIChGate_Maciokas02 : public VIonGate {
public:
  SIChGate_Maciokas02(void) { k=1; }

  double tau(double ) { return TSCALE*5e-6; }
  double infty(double V)  { return 1/(1 + exp(( VSCALE * (V - *Vresting) - 15)/2.5)); }

  IONGATE_TABLES(SIChGate_Maciokas02);
};

//! Voltage dependent ion channel from Maciokas et al. 2002
/** Uses SICmGate_Maciokas02 and SIChGate_Maciokas02

 Reference: Maciokas, J., Goodman, P., Kenyon, J., Accurate Dynamical Model of Interneuronal GABAergic Channel Physiologies
*/
class SICChannel_Maciokas02 : public ActiveChannel {

  DO_REGISTERING

public:

  SICChannel_Maciokas02(void) {
    addGate(new SICmGate_Maciokas02);
    addGate(new SIChGate_Maciokas02);

    Erev = -80; // [mV] biological value
    // Transform to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~SICChannel_Maciokas02(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }
};



//== Channels of neocortical pyramidal neurons from the SenseLab ModelDB archiv ========



//! Voltage dependent ion gate from Korngreen et al. 2002
/** Used by AChannel_Korngreen02

Reference: Korngreen A, Sakmann B.Voltage-gated K+ channels in layer 5 neocortical pyramidal neurones from young rats: subtypes and gradients. J Physiol. 2000 Jun 15;525 Pt 3:621-39. */
class AnGate_Korngreen02 : public VIonGate {
public:
  AnGate_Korngreen02(void) { k=2;}

  double tau(double V) { TRANSFORM_V;
  			 return (V < -50) ? 1e-3 * (1.25+175.03*exp(V*0.026)) : 1e-3 * (1.25+13*exp(-V*0.026)); }
  double infty(double V) { TRANSFORM_V;
  			   return 1/(1+exp(-(V+14)/14.6)); }

  IONGATE_TABLES(AnGate_Korngreen02);
};

//! Voltage dependent ion gate from Korngreen et al. 2002
/** Used by AChannel_Korngreen02

Reference: Korngreen A, Sakmann B.Voltage-gated K+ channels in layer 5 neocortical pyramidal neurones from young rats: subtypes and gradients. J Physiol. 2000 Jun 15;525 Pt 3:621-39. */
class AlGate_Korngreen02 : public VIonGate {
public:
  AlGate_Korngreen02(void) { k=1; }

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * (360+(1010+24*(V+55))*exp(-pow((V+75)/48,2))); }
  double infty(double V)  { TRANSFORM_V;
  			    return 1/(1+exp((V+54)/11)); }

  IONGATE_TABLES(AlGate_Korngreen02);
};


//! Voltage dependent ion channel from Korngreen et al. 2002
/** Uses AnGate_Korngreen02 and AlGate_Korngreen02

Reference: Korngreen A, Sakmann B.Voltage-gated K+ channels in layer 5 neocortical pyramidal neurones from young rats: subtypes and gradients. J Physiol. 2000 Jun 15;525 Pt 3:621-39. */
class AChannel_Korngreen02 : public ActiveChannel {

  DO_REGISTERING

public:

  AChannel_Korngreen02(void) {
    addGate(new AnGate_Korngreen02);
    addGate(new AlGate_Korngreen02);
    Ts = 1;
    Gbar = 8e-3*A_MEMBRANE; // [S/cm2]*[cm2]
    Erev = -80;  // (original -90) [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~AChannel_Korngreen02(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};


//! Voltage dependent ion gate from Korngreen et al. 2002
/** Used by KChannel_Korngreen02

Reference: Korngreen A, Sakmann B.Voltage-gated K+ channels in layer 5 neocortical pyramidal neurones from young rats: subtypes and gradients. J Physiol. 2000 Jun 15;525 Pt 3:621-39. */
class KnGate_Korngreen02 : public VIonGate {
public:
  KnGate_Korngreen02(void) { k=4;}

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * (0.34 + 0.92*exp(-pow((V+71)/59,2))); }
  double infty(double V) { TRANSFORM_V;
  			   return 1/(1+exp(-(V+47)/29)); }

  IONGATE_TABLES(KnGate_Korngreen02);
};

//! Voltage dependent ion gate from Korngreen et al. 2002
/** Used by KChannel_Korngreen02

Reference: Korngreen A, Sakmann B.Voltage-gated K+ channels in layer 5 neocortical pyramidal neurones from young rats: subtypes and gradients. J Physiol. 2000 Jun 15;525 Pt 3:621-39. */
class KlGate_Korngreen02 : public VIonGate {
public:
  KlGate_Korngreen02(void) { k=1; }

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * (8 + 49*exp(-pow((V+73)/23,2))); }
  double infty(double V)  { TRANSFORM_V;
  			    return 1/(1+exp((V+66)/10)); }

  IONGATE_TABLES(KlGate_Korngreen02);
};


//! Voltage dependent ion channel from Korngreen et al. 2002
/** Uses KnGate_Korngreen02 and KlGate_Korngreen02

Reference: Korngreen A, Sakmann B.Voltage-gated K+ channels in layer 5 neocortical pyramidal neurones from young rats: subtypes and gradients. J Physiol. 2000 Jun 15;525 Pt 3:621-39. */
class KChannel_Korngreen02 : public ActiveChannel {

  DO_REGISTERING

public:

  KChannel_Korngreen02(void) {
    addGate(new KnGate_Korngreen02);
    addGate(new KlGate_Korngreen02);
    Ts = 1;
    Gbar = 8e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = -80;  // (original -90) [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~KChannel_Korngreen02(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

//! Voltage dependent ion gate from McCormick and Huguenard (1992)
/**  Used by NPChannel_McCormick02

     Reference: McCormick DA, Huguenard JR.
     A model of the electrophysiological properties of thalamocortical relay neurons.
     J Neurophysiol 1992 Oct;68(4):1384-400

     See also: Alexander D. Protopapas, Michael Vanier and James M. Bower. Chapter 12: "Simulating Large Networks of Neurons" in Methods in Neuronal Modeling: From Ions to Networks, 2nd edition, Christof Koch and Idan Segev, eds, MIT Press, Cambridge, Mass., 1998
*/
class NPmGate_McCormick92 : public VIonGate {
public:
  NPmGate_McCormick92(void) { k=1; }

  double alpha(double V) { TRANSFORM_V;
  			 return 1e3 * (0.091*(V+48)/(1.0-exp(-(V+48.0)/5.0))); }
  double beta(double V) { TRANSFORM_V;
  			 return 1e3 * (-0.062*(V+48.0)/(1.0-exp((V+48.0)/5.0))); }
  double infty(double V)  { TRANSFORM_V;
  			    return 1.0/(1.0+exp(-(49.0+V)/5)); }

  IONGATE_TABLES(NPmGate_McCormick92);
};


//! Voltage dependent ion channel from McCormick and Huguenard (1992)
/**  Uses NPmGate_McCormick92

     Reference: McCormick DA, Huguenard JR.
     A model of the electrophysiological properties of thalamocortical relay neurons.
     J Neurophysiol 1992 Oct;68(4):1384-400

     See also: Alexander D. Protopapas, Michael Vanier and James M. Bower. Chapter 12: "Simulating Large Networks of Neurons" in Methods in Neuronal Modeling: From Ions to Networks, 2nd edition, Christof Koch and Idan Segev, eds, MIT Press, Cambridge, Mass., 1998
*/
class NPChannel_McCormick02 : public ActiveChannel {

  DO_REGISTERING

public:

  NPChannel_McCormick02(void) {
    addGate(new NPmGate_McCormick92);
    Ts = 1;
    Gbar = 2.2e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = 55.0;  // [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~NPChannel_McCormick02(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

//! Voltage dependent ion gate from Mainen and Sejnowski (1996)
/** Used by MChannel_Mainen96

    Reference: Mainen ZF, Sejnowski TJ. Influence of dendritic structure on firing pattern in model neocortical neurons. Nature. 1996 Jul 25;382(6589):363-6.
*/
class MnGate_Mainen96 : public VIonGate {
public:
  MnGate_Mainen96(void) { k=1; }

  double alpha(double V) { TRANSFORM_V;
  			 return 1e3 * (0.001*(V+30)/(1-exp(-(V+30)/9))); }
  double beta(double V) { TRANSFORM_V;
  			 return 1e3 * (-0.001*(V+30)/(1-exp((V+30)/9))); }

  IONGATE_TABLES(MnGate_Mainen96);
};


//! Voltage dependent ion channel from Mainen and Sejnowski (1996)
/** Uses MnGate_Mainen96

    Reference: Mainen ZF, Sejnowski TJ. Influence of dendritic structure on firing pattern in model neocortical neurons. Nature. 1996 Jul 25;382(6589):363-6.
*/
class MChannel_Mainen96 : public ActiveChannel {

  DO_REGISTERING

public:

  MChannel_Mainen96(void) {
    addGate(new MnGate_Mainen96);
    Ts = 1;
    Gbar = 1e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = -80.0;  // (original -90) [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~MChannel_Mainen96(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

#define CELSIUS 30

//! Voltage dependent ion gate from Stuart and Spruston (1998)
/** Used by HChannel_Stuart98

    Reference: Stuart G, Spruston N. Determinants of voltage attenuation in neocortical pyramidal neuron dendrites. J Neurosci. 1998 May 15;18(10):3501-10.
*/
class HnGate_Stuart98 : public VIonGate {
public:
  HnGate_Stuart98(void) { k=1; }

  double alpha(double V) { return  exp(1e-3*3*(V+88)*9.648e4/(8.315*(273.16+CELSIUS))); }
  double beta(double V) { return  exp(1e-3*3*0.4*(V+88)*9.648e4/(8.315*(273.16+CELSIUS))); }
  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * (beta(V)/(0.00057+0.00057*alpha(V))); }
  double infty(double V) { TRANSFORM_V;
  			 return  1/(1+alpha(V)); }

  IONGATE_TABLES(HnGate_Stuart98);
};


//! Voltage dependent ion channel from Stuart and Spruston (1998)
/** Uses HnGate_Stuart98

    Reference: Stuart G, Spruston N. Determinants of voltage attenuation in neocortical pyramidal neuron dendrites. J Neurosci. 1998 May 15;18(10):3501-10.
*/
class HChannel_Stuart98 : public ActiveChannel {

  DO_REGISTERING

public:

  HChannel_Stuart98(void) {
    addGate(new HnGate_Stuart98);
    Ts = 1;
    Gbar = 3e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = -35.0;  // [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~HChannel_Stuart98(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

#define SECH(_V_)  2/(exp(2*_V_)+exp(-2*_V_))

//! Voltage dependent ion gate from Brown et al. (1993)
/** Used by HVACAChannel_Brown93

 Reference: Brown AM, Schwindt PC, Crill WE. Voltage dependence and activation kinetics of pharmacologically defined components of the high-threshold calcium current in rat neocortical neurons. J Neurophysiol. 1993 Oct;70(4):1530-43.

 See also: Seamans JK, Durstewitz D, Christie BR, Stevens CF, Sejnowski TJ. Dopamine D1/D5 receptor modulation of excitatory synaptic inputs to layer V prefrontal cortex neurons. Proc Natl Acad Sci U S A. 2001 Jan 2;98(1):301-6.
*/
class HVACAuGate_Brown93 : public VIonGate {
public:
  HVACAuGate_Brown93(void) { k=2;}

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * (1.25*SECH(-0.031*(V+37.1))); }
  double infty(double V) { TRANSFORM_V;
  			   return 1/(1+exp(-(V+24.6)/11.3)); }

  IONGATE_TABLES(HVACAuGate_Brown93);
};

//! Voltage dependent ion gate from Brown et al. (1993)
/** Used by HVACAChannel_Brown93

 Reference: Brown AM, Schwindt PC, Crill WE. Voltage dependence and activation kinetics of pharmacologically defined components of the high-threshold calcium current in rat neocortical neurons. J Neurophysiol. 1993 Oct;70(4):1530-43.

 See also: Seamans JK, Durstewitz D, Christie BR, Stevens CF, Sejnowski TJ. Dopamine D1/D5 receptor modulation of excitatory synaptic inputs to layer V prefrontal cortex neurons. Proc Natl Acad Sci U S A. 2001 Jan 2;98(1):301-6.
*/
class HVACAvGate_Brown93 : public VIonGate {
public:
  HVACAvGate_Brown93(void) { k=1; }

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * 420; }
  double infty(double V)  { TRANSFORM_V;
  			    return 1/(1+exp((V+12.6)/18.9)); }

  IONGATE_TABLES(HVACAvGate_Brown93);
};


//! Voltage dependent ion channel from Brown et al. (1993)
/** Uses HVACAuGate_Brown93 and HVACAvGate_Brown93

 Reference: Brown AM, Schwindt PC, Crill WE. Voltage dependence and activation kinetics of pharmacologically defined components of the high-threshold calcium current in rat neocortical neurons. J Neurophysiol. 1993 Oct;70(4):1530-43.

 See also: Seamans JK, Durstewitz D, Christie BR, Stevens CF, Sejnowski TJ. Dopamine D1/D5 receptor modulation of excitatory synaptic inputs to layer V prefrontal cortex neurons. Proc Natl Acad Sci U S A. 2001 Jan 2;98(1):301-6.
*/
class HVACAChannel_Brown93 : public ActiveCaChannel {

  DO_REGISTERING

public:

  HVACAChannel_Brown93(void) {
    addGate(new HVACAuGate_Brown93);
    addGate(new HVACAvGate_Brown93);
    Ts = 1;
    Gbar = 0.3e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = 140;  // [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~HVACAChannel_Brown93(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};


//! Voltage dependent ion gate from Destexhe et al. (1998)
/** Used by CALChannel_Destexhe98

   Reference: Destexhe A, Contreras D, Steriade M. Mechanisms underlying the synchronizing action of corticothalamic feedback through inhibition of thalamic relay cells. J Neurophysiol. 1998 Feb;79(2):999-1016.
*/
class CALmGate_Destexhe98 : public VIonGate {
public:
  CALmGate_Destexhe98(void) { k=2;}

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * 0.1; } /* Originally 0 */
  double infty(double V) { TRANSFORM_V;
  			   return 1.0/(1+exp(-(V+57)/6.2)); }

  IONGATE_TABLES(CALmGate_Destexhe98);
};

//! Voltage dependent ion gate from Destexhe et al. (1998)
/** Used by CALChannel_Destexhe98

   Reference: Destexhe A, Contreras D, Steriade M. Mechanisms underlying the synchronizing action of corticothalamic feedback through inhibition of thalamic relay cells. J Neurophysiol. 1998 Feb;79(2):999-1016.
*/
class CALhGate_Destexhe98 : public VIonGate {
public:
  CALhGate_Destexhe98(void) { k=1; }

  double tau(double V) { TRANSFORM_V;
  			 return 1e-3 * (30.8 + (211.4 + exp((V+113.2)/5))/(1+exp((V+84)/3.2))); }
  double infty(double V)  { TRANSFORM_V;
  			    return 1.0/(1+exp((V+81)/4.0)); }

  IONGATE_TABLES(CALhGate_Destexhe98);
};


//! Voltage dependent ion channel from Destexhe et al. (1998)
/** Uses CALmGate_Destexhe98 and CALhGate_Destexhe98

   Reference: Destexhe A, Contreras D, Steriade M. Mechanisms underlying the synchronizing action of corticothalamic feedback through inhibition of thalamic relay cells. J Neurophysiol. 1998 Feb;79(2):999-1016.
*/
class CALChannel_Destexhe98 : public ActiveChannel {

  DO_REGISTERING

public:

  CALChannel_Destexhe98(void) {
    addGate(new CALmGate_Destexhe98);
    addGate(new CALhGate_Destexhe98);
    Ts = 1;
    Gbar = 2e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = 140;  // [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~CALChannel_Destexhe98(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

//! Voltage dependent ion gate from Mainen and Sejnowski (1996)
/** Used by KCAChannel_Mainen96

    Reference: Mainen ZF, Sejnowski TJ. Influence of dendritic structure on firing pattern in model neocortical neurons. Nature. 1996 Jul 25;382(6589):363-6.
*/
class KCAnGate_Mainen96 : public ConcIonGate {
public:
  KCAnGate_Mainen96(void) { k=1; ConcType=CA;}


  //! The activation parameter \f$\alpha(V) \in [0,1]\f$
  double alpha(double C) { return 0.01*C*1e3; }
  //! The activation parameter \f$\beta(V) \in [0,1]\f$
  double beta(double ) { return 0.02; }

  double tau(double C) { return 1e-3/(alpha(C) + beta(C)); }
  double infty(double C) { return alpha(C)/(alpha(C) + beta(C)); }

  IONGATE_TABLES(KCAnGate_Mainen96);
};


//! Voltage dependent ion gate from Mainen and Sejnowski (1996)
/** Uses KCAnGate_Mainen96

    Reference: Mainen ZF, Sejnowski TJ. Influence of dendritic structure on firing pattern in model neocortical neurons. Nature. 1996 Jul 25;382(6589):363-6.
*/
class KCAChannel_Mainen96 : public ActiveChannel {

   DO_REGISTERING

public:

  KCAChannel_Mainen96(void) {
    addGate(new KCAnGate_Mainen96);
    Ts = 1;
    Gbar = 1e-3*A_MEMBRANE; // [S/cm2]*[cm2]

    Erev = -80;  // (original -90) [mV] biological value
    // Transform biological Erev value to model voltage range
    TRANSFORM_EREV;
  }

  virtual ~KCAChannel_Mainen96(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

//== Channels used by Destexhe take from Traub et al. and Mainen et al. ==========================================================

#define Amembrane 34636e-12 // [m2]

// #define NUMMETHOD 0
#define NUMMETHOD 1

// #define GSCALE 0.4
#define GSCALE 1
#define ABSCALE 1e3
#define TADJM  2.9529 //  = 2.3 ^ ((CELSIUS-23)/10), CELSIUS = 36
#define VT   -63 + 1e-5  // [mV], -58 mV needed for Vthresh adjustment
// #define VT   -58 + 1e-5  // [mV], -58 mV needed for Vthresh adjustment

//! From: Traub RD, Miles R (1991): Neuronal Networks of the Hippocampus, Cambridge University Press, New York 
class NAmGate_Traub91 : public VIonGate {
public:
  NAmGate_Traub91(void) { k=3;  nummethod = NUMMETHOD;}

  double alpha(double V) { V = V*1e3;
  			 return ABSCALE*-0.32 * (V - VT - 13)/(exp(-(V-VT-13)/4)-1); }
  double beta(double V) { V = V*1e3;
  			 return ABSCALE*0.28*(V-VT-40)/(exp((V-VT-40)/5)-1); }

  IONGATE_TABLES(NAmGate_Traub91);
};

//! From: Traub RD, Miles R (1991): Neuronal Networks of the Hippocampus, Cambridge University Press, New York 
class NAhGate_Traub91 : public VIonGate {
public:
  NAhGate_Traub91(void) { k=1;  nummethod = NUMMETHOD;}

  double alpha(double V) { V = V*1e3;
  			 return ABSCALE*0.128 * exp(-(V-VT+10-17)/18); }
  double beta(double V) { V = V*1e3;
  			 return ABSCALE*4 / (1+exp(-(V-VT+10-40)/5)); }

  IONGATE_TABLES(NAhGate_Traub91);
};

class NAChannel_Traub91 : public ActiveChannel {

  DO_REGISTERING

public:


  NAChannel_Traub91(void) {
    addGate(new NAmGate_Traub91);
    addGate(new NAhGate_Traub91);
    Ts = 1;
//    Gbar = GSCALE * 30 * Amembrane; // [S/m2]*[m2]
//    Gbar = GSCALE * 360 * Amembrane; // [S/m2]*[m2]

//    Gbar = GSCALE * 120 * Amembrane; // [S/m2]*[m2]
    Gbar = 4.3* GSCALE * 120 * Amembrane; // [S/m2]*[m2]
//    Gbar = 1.67* GSCALE * 120 * Amembrane; // [S/m2]*[m2]
    Erev = 50e-3;  // [mV]
  }

  virtual ~NAChannel_Traub91(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};


//! From: Traub RD, Miles R (1991): Neuronal Networks of the Hippocampus, Cambridge University Press, New York 
class KDnGate_Traub91 : public VIonGate {
public:
  KDnGate_Traub91(void) { k=4;  nummethod = NUMMETHOD;}

  double alpha(double V) { V = V*1e3;
  			 return ABSCALE*-0.032*(V-VT-15)/(exp(-(V-VT-15)/5)-1); }
  double beta(double V) { V = V*1e3;
  			 return ABSCALE*0.5*exp(-(V-VT-10)/40); }

  IONGATE_TABLES(KDnGate_Traub91);
};

class KDChannel_Traub91 : public ActiveChannel {

  DO_REGISTERING

public:

  // original model has Vrest = -80 mV, Vwork = -65 mV, Vthresh = -55 mV
  // shift V by +10 results in Vrest = -70 mV, Vwork = -55 mV, Vthresh = -45 mV
  // what is approx. the above defined biological working range needed for parameter rescaling


  KDChannel_Traub91(void) {
    addGate(new KDnGate_Traub91);
    Ts = 1;
//    Gbar = GSCALE * 50 * Amembrane; // [S/m2]*[m2]
//    Gbar = GSCALE * 70 * Amembrane; // [S/m2]*[m2]
    Gbar = GSCALE * 100 * Amembrane; // [S/m2]*[m2]
    Erev = -90e-3; // [mV]
  }

  virtual ~KDChannel_Traub91(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};




//! From: Mainen ZF, Joerges J, Huguenard JR, Sejnowski TJ.,A model of spike initiation in neocortical pyramidal neurons.,Neuron. 1995 Dec;15(6):1427-39.
class MpGate_Mainen96orig : public VIonGate {
public:
  MpGate_Mainen96orig(void) { k=1; nummethod = NUMMETHOD;}

  double alpha(double V) { V = V*1e3;
  			 return TADJM*ABSCALE*0.0001*(V+30)/(1-exp(-(V+30)/9)); }
  double beta(double V) { V = V*1e3;
  			 return TADJM*ABSCALE*-0.0001*(V+30)/(1-exp((V+30)/9)); }

  IONGATE_TABLES(MpGate_Mainen96orig);
};

class MChannel_Mainen96orig : public ActiveChannel {

  DO_REGISTERING

public:


  MChannel_Mainen96orig(void) {
    addGate(new MpGate_Mainen96orig);
    Ts = 1;
//    Gbar = 0  * Amembrane; // [S/m2]*[m2]
    Gbar = GSCALE * 5 * Amembrane; // [S/m2]*[m2]
    Erev = -90e-3; // [V]
  }

  virtual ~MChannel_Mainen96orig(void) {
    for(int i=0;i<nGates;i++)
      delete gates[i];
  }

  virtual int updateInternal(void);

  //! Scale of the time constants of the gating variables \f$\tau(V)\f$ [range=(0,1e2); readwrite;]
  double Ts;
};

#endif



