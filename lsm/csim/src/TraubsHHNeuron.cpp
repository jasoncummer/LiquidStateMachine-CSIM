#include "TraubsHHNeuron.h"

// For each gate we need this static members
double *Traubs_HH_h_Gate::C1=0;
double *Traubs_HH_h_Gate::C2=0;

double *Traubs_HH_m_Gate::C1=0;
double *Traubs_HH_m_Gate::C2=0;

double *Traubs_HH_n_Gate::C1=0;
double *Traubs_HH_n_Gate::C2=0;

/** We set the default mebrane properties */
TraubsHHNeuron::TraubsHHNeuron(void)
{

  Rm       = 100*1e6;   // Ohm
  Cm       = 2*1e-10;   // Farad
  Vresting = -0.060;    // Volt
  Vtr      = -0.063;    // Volt
  Vinit    = Vresting;  // Start integration at the resting potential
  Trefract = 0.005;     // seconds
  Inoise   = 0.0;       // no noise in the model
  Iinject  = 0.0;       // no constant current injection

  // If Vm exceeds Vthresh a spike is transitted to postsynaptic neurons
  Vthresh=-0.050;

  // Since we have spike generating channels we do not want Vm 
  // to be reset after a spike. Hence we set doRest = 0
  doReset=0;

  // We have to make sure that these pointers are initialized properly
  k  = NULL;
  na = NULL;
}

/** Here the Na nd K channel are generated and connected to the neuron itself*/
int TraubsHHNeuron::init(Advancable *a)
{
  /* 
    gNa = 0.1 S/cm2
    gKd = 0.03 S/cm2
    ENa = 50 mV
    EK = -90 mV
  */

  // Flag which indicates an error
  bool e = 0;

  // Create the channels
  k  = new Traubs_HH_K_Channel(0.06*1e-4, -0.09, Vtr);
  na = new Traubs_HH_Na_Channel(0.2*1e-4, 0.05, Vtr);

  // Connect the K channel to the neuron
  e = e || a->addIncoming(k) < 0;
  e = e || k->addIncoming(a) < 0;
  e = e || a->addOutgoing(k) < 0;
  e = e || k->addOutgoing(a) < 0;

  // Connect the Na channel to the neuron
  e = e || a->addIncoming(na) < 0;
  e = e || na->addIncoming(a) < 0;
  e = e || a->addOutgoing(na) < 0;
  e = e || na->addOutgoing(a) < 0;
 
  return e ? -1 : 0;

}

/** We have to call updateInternal of the Na and K channel */
int TraubsHHNeuron::updateInternal(void)
{
  bool e=0;
  e = e || k->updateInternal() < 0;
  e = e || na->updateInternal() < 0;
  return e;
}

/** We have to delete the Na and K channel */
TraubsHHNeuron::~TraubsHHNeuron(void)
{
  if ( k  ) { delete k;  k=0;  }
  if ( na ) { delete na; na=0; }
}
