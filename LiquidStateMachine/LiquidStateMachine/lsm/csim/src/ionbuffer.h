/*! \file ionbuffer.h
**  \brief Class definition of MembranePatch
*/

#ifndef _IONBUFFER_H_
#define _IONBUFFER_H_

#include "ionchannel.h"

#define FARADAY 96589	// Faraday constant [Coulomb]
#define R 8.31		// universal gas constant [Joules/Mol]
#define DEPTH 0.1e-6	// thickness of the intracellular perimembrane shell [m]

#define V_REST_BIOL   -70e-3    // [V]
#define V_THRESH_BIOL -40e-3    // [V]

class IonChannel;

//! An ion buffer handling the ion concentration of multiple ionic input currents and outward pump.
/**
 ** Missing ...
 ***/
class IonBuffer {

 public:

  IonBuffer(float ConcRest_def, float ConcOut_def, float Tau_def, float Temp_def, int zIon_def);

  virtual ~IonBuffer();

  //! Reset the IonBuffer.
  void reset(double *Gtot, double *Ich, double Vresting, double VmScale);

  //! Calculate for this class of ion channels the current and conductance and if necessary Erev and Conc
  void nextstate(double *Imtot, double *Gmtot, double Vm, double Vresting, double VmScale);

  //! Get pointer to the ion concentration variable
  void getConc(double **C) {  ConcActive = 1; (*C) =  &(this->Conc);}

  //! Get pointer to the ion concentration variable
  void getErev(double **E) { ConcActive = 1; ErevActive = 1; (*E) =  &(this->Erev);}

  //! Resting interior ion concentration [Ca]; [units=Mol; range=(0,1); readwrite;]
  float ConcRest;

  //! Exterior ion concentration [Ca]; [units=Mol; range=(0,1); readwrite;]
  float ConcOut;

  //! Scales the ion concentration parameter tables; [units=; range=(0,1e5); readwrite;]
  float ConcScale;

  //! Ion concentration decay time constant; [units=sec; range=(0,1e3); readwrite;]
  float Tau;

   //! Temperature; [units=Kelvin; range=(0,1e3); readwrite;]
  float Temp;

 protected:
  friend class MembranePatch;


  //! The membrane capacity \f$C_m\f$ [range=(0,1); units=;]
  double Conc;

  //! The membrane capacity \f$C_m\f$ [range=(0,1); units=;]
  double Erev;

  //! True if ion buffer has to be calculated.
  bool active;

  //! True if ion buffers Conc variable has to be calculated.
  bool ConcActive;

  //! True if ion buffers Erev variable has to be calculated.
  bool ErevActive;

  //! Atomic number of the ion
  int zIon;

  //! Internal constant for the exponential Euler integration of Conc.
  double C1;

  //! Number of channels [readonly; units=;]
  int nChannels;

  //! Length of list of channels [hidden]
  int lChannels;

  //! A list of associated channels [hidden]
  IonChannel **channels;

  //! Call to add a new channel
  void addChannel(IonChannel *newChannel);

  //! Set reversal potential
  void SetErev(double Vresting, double VmScale);

};

#endif
