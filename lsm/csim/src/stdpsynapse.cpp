/*! \file stdpsynapse.cpp
**  \brief Implementation of StdpSynapse
*/

#include <math.h>
#include "stdpsynapse.h"
#include "spikingneuron.h"
#include "csimerror.h"

StdpSynapse::StdpSynapse(void)
{
  // Tmax = (float) 50e-3;
  // Tmin = (float)-50e-3;
  Apos = 0.5;
  Aneg = -0.5;
  STDPgap = (float) 2e-3;
  activeSTDP = 1;
  useFroemkeDanSTDP = 1;
  dw = 0;
  back_delay = 0.0;
}


#define POST ((SpikingNeuron *)post)
#define PRE ((SpikingNeuron *)pre)

//! Called if the postsynaptic spikes hits (arrives at) the synapse
/** Calls the learning function for each pair of pre-post spikes and
 ** than does the usual increase of the PSR */
int StdpSynapse::preSpikeHit(void) {
  //printf("STDP: preSpikeHit -- enter, %g\n",SimulationTime);

  // call the learning function stdpLearning() for each pair of
  // pre-post spikes
  if (activeSTDP) {
    // printf("PRE->nSpikes(): %i\n", PRE->nSpikes());
    // printf("POST->nSpikes(): %i\n", POST->nSpikes());
    double epre, epost;
    if ((PRE->nSpikes() > 0) && (useFroemkeDanSTDP))
      epre = 1 - exp(-(SimulationTime-PRE->spikeTime((PRE->nSpikes())-2))/tauspre);
    else
      epre = 1;
    int    actPostSpike = (POST->nSpikes())-1;
    double delta;
    while ((actPostSpike >= 0) &&
          ((delta = POST->spikeTime(actPostSpike) - SimulationTime) > -3*tauneg)) {
      if ((actPostSpike > 0) && (useFroemkeDanSTDP)) 
        epost = 1 - exp(-(POST->spikeTime(actPostSpike) - POST->spikeTime(actPostSpike-1))/tauspost);
      else
        epost = 1;
      stdpLearning(delta, epost, epre);
      // stdpLearning(delta, 1, 1);
      --actPostSpike;
    }
  }

  // and now do the basic stuff, i.e. increment the psr
  stdpChangePSR();

  //printf("STDP: preSpikeHit -- leave\n");

  // return 1 or 0 depending on whether to activate the synapse
  NEED_TO_ACTIVATE;
}


//! Called if the postsynaptic spikes hits (arrives at) the synapse
/** Call the learning function for each pair of post-pre spikes */
int StdpSynapse::postSpikeHit(void) {
  //printf("STDP: postSpikeHit -- enter, %g\n",SimulationTime);
  //printf("STDP: postSpikeHit -- enter (SimTime: %f, delay: %f): \n", SimulationTime, delay);
  //   int testprespike = (PRE->nSpikes())-1;
  //   while (testprespike >= 0) {
  //     printf("%i, %f, %f, %f\n", testprespike, PRE->spikeTime(testprespike),
  //       PRE->spikeTime(testprespike)+delay, SimulationTime-PRE->spikeTime(testprespike));
  //     testprespike--;
  //   }
  // call the learning function stdpLearning() for each pair of
  // post-pre spikes
  
  if (activeSTDP) {
    double epre, epost;
    if ((POST->nSpikes() > 0) && (useFroemkeDanSTDP))
      epost = 1 - exp(-(SimulationTime - POST->spikeTime((POST->nSpikes())-2))/tauspost);
    else
      epost = 1;
    int    actPreSpike = (PRE->nSpikes())-1;
    double delta;
    while ((actPreSpike >= 0) &&
          ((delta = SimulationTime - (PRE->spikeTime(actPreSpike)+delay)) > 0) &&
          (delta < 3*taupos)) {
      if ((actPreSpike > 0) && (useFroemkeDanSTDP))
        epre = 1 - exp(-(PRE->spikeTime(actPreSpike) - PRE->spikeTime(actPreSpike-1))/tauspre);
      else
        epre = 1;
      stdpLearning(delta, epost, epre);
      // stdpLearning(delta, 1, 1);
      --actPreSpike;
    }
  }
  return 0;
  // printf("STDP: postSpikeHit -- leave\n");
}



void StdpSynapse::stdpLearning(double delta, double epost, double epre)
{
  // double dw = 0;
  //printf("%f %f %f -> %f\n", delta, epost, epre, dw);
  if (delta < -STDPgap) {
    // Depression
    // dw = Aneg * exp(delta/(fabs(Tmin)/3));
    dw = pow(W, muneg) * Aneg * exp(delta/tauneg);
  } else if (delta > STDPgap) {
    // Potentiation
    // dw = Apos * exp(-delta/(fabs(Tmax)/3));
    dw = pow(Wex-W, mupos) * Apos * exp(-delta/taupos);
  } else {
    return;
  }
  // printf("%f.15\n", dw);
  W += epost * epre * dw;
  // check the sign
  if ((Wex < 0 && W >0) || (Wex >0 && W<0)) W = 0;
  // check for greater Wmax
  if (fabs(W) > fabs(Wex)) W = Wex;
  // csimPrintf("delta=%g\n",delta);
  //printf("%e\n", dw);
  // printf("%f %f %f -> %e (%e)\n", delta, epost, epre, dw, W);
}


//! Connect the presynaptic neuron
/** \internal nothing else to do than a SpikingSynapse */
int StdpSynapse::addIncoming(Advancable *a)
{
  // nothing else to do than a SpikingSynapse
  return SpikingSynapse::addIncoming(a);
}


//! Connect the postsynaptic neuron
/** \internal Here we have to check wheter the postsynaptic neuron is also a spiking neuron */
int StdpSynapse::addOutgoing(Advancable *a)
{
  SpikingNeuron *n = dynamic_cast<SpikingNeuron *>(a);
  if (n) {
    return SpikingSynapse::addOutgoing(a);
  } else {
    TheCsimError.add("StdpSynapse::addOutgoing (idx=%i): postsynaptic element must be a spiking neuron (not a %s)!\n",
		     getId(),a->className());
    return -1;
  }
}


#undef POST
#undef PRE



