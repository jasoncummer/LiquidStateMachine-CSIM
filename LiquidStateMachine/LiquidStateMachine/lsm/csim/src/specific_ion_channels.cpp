
#include "specific_ion_channels.h"
#include "csimerror.h"
#include "cbneuron.h"
#include "analoginputneuron.h"


// RENOs ion channels

double *AmGate_Hoffman97::C1=0;
double *AmGate_Hoffman97::C2=0;
double *AhGate_Hoffman97::C1=0;
double *AhGate_Hoffman97::C2=0;

double *MmGate_Wang98::C1=0;
double *MmGate_Wang98::C2=0;

double *SICmGate_Maciokas02::C1=0;
double *SICmGate_Maciokas02::C2=0;
double *SIChGate_Maciokas02::C1=0;
double *SIChGate_Maciokas02::C2=0;

// Senselab ModelDB neocortical pyramidal neuron channels

double *AnGate_Korngreen02::C1=0;
double *AnGate_Korngreen02::C2=0;
double *AlGate_Korngreen02::C1=0;
double *AlGate_Korngreen02::C2=0;

double *KnGate_Korngreen02::C1=0;
double *KnGate_Korngreen02::C2=0;
double *KlGate_Korngreen02::C1=0;
double *KlGate_Korngreen02::C2=0;

double *NPmGate_McCormick92::C1=0;
double *NPmGate_McCormick92::C2=0;

double *MnGate_Mainen96::C1=0;
double *MnGate_Mainen96::C2=0;

double *HnGate_Stuart98::C1=0;
double *HnGate_Stuart98::C2=0;

double *HVACAuGate_Brown93::C1=0;
double *HVACAuGate_Brown93::C2=0;
double *HVACAvGate_Brown93::C1=0;
double *HVACAvGate_Brown93::C2=0;

double *CALmGate_Destexhe98::C1=0;
double *CALmGate_Destexhe98::C2=0;
double *CALhGate_Destexhe98::C1=0;
double *CALhGate_Destexhe98::C2=0;

double *KCAnGate_Mainen96::C1=0;
double *KCAnGate_Mainen96::C2=0;

// Destexhe take from Traub et al. 

double *NAmGate_Traub91::C1=0;
double *NAmGate_Traub91::C2=0;
double *NAhGate_Traub91::C1=0;
double *NAhGate_Traub91::C2=0;

double *KDnGate_Traub91::C1=0;
double *KDnGate_Traub91::C2=0;

double *MpGate_Mainen96orig::C1=0;
double *MpGate_Mainen96orig::C2=0;

//==================================================================================

int CaGate_Yamada98::updateInternal(void) {
  // get the static table pointers and store them locally
  c1=getC1();
  c2=getC2();

  // allocate memory
  bool haveToInitTable = 1;

  if (!c1) { setC1(c1=(double *)malloc(IONGATE_CA_TABLE_SIZE*sizeof(double)));}
  if (!c2) { setC2(c2=(double *)malloc(IONGATE_CA_TABLE_SIZE*sizeof(double)));}

  //
  // set up the look up tables
  //
  if ( haveToInitTable ) {
    int i; double ca; double *p1,*p2;
    for(ca=IONGATE_CA_MIN,i=0,p1=c1,p2=c2;i<IONGATE_CA_TABLE_SIZE;i++,ca+=IONGATE_CA_INC,p1++,p2++) {
      (*p1) = exp(-DT/tau(ca));
      (*p2) = (1.0-(*p1))*infty(ca);
#ifdef _GUN_SOURCE
      if ( !finite((*p1)) ) {
	TheCsimError.add("CaGate_Yamada98::reset: There occurred undefined values (NaN or Inf) for C1!\n");
	return -1;
      }
      if ( !finite((*p2)) ) {
	TheCsimError.add("CaGate_Yamada98::reset: There occurred undefined values (NaN or Inf) for C2!\n");
	return -1;
      }
#endif
    }
  }
  return 0;
}

void CaGate_Yamada98::reset(void)
{
if ( Ca!=0 ) {
//   if ( ( *Ca < IONGATE_CA_MIN ) || ( IONGATE_CA_MAX < *Ca ) ) {
//      TheCsimError.add("CaGate_Yamada98::reset: [Ca] concentration (%g Mol) out of range (%g,%g)!\n",
//         			*Ca,IONGATE_CA_MIN,IONGATE_CA_MAX);
//      return;
//    }
    // set p to the 'resting value' at time t=0
    p = infty(*Ca);
  } else {
    TheCsimError.add("CaGate_Yamada98::reset: IonGate not connected to any [Ca] concentration!\n");
    return;
  }

  // calculate output
  P = ( k!=1 ) ? pow(p,k) : p;

}

int CaGate_Yamada98::advance(void)
{
  // do the table lookup
  int i=(int)((*Ca-IONGATE_CA_MIN)/IONGATE_CA_INC + 0.5);

  // exponential euler integratio step
  if ( i < 0 ) {
    // Do the correct calculation:
    double C1,C2;
    C1 = exp(-DT/tau(*Ca));
    C2 = (1.0-C1)*infty(*Ca);
    p = C1*p + C2;

    // Ca < IONGATE_CA_MIN
//    p = c1[0]*p + c2[0];
//    csimPrintf("CaGate_Yamada98::advance: [Ca] concentration (%g Mol) out of range (%g,%g)!\n",*Ca,IONGATE_CA_MIN,IONGATE_CA_MAX);
  } else if ( i < IONGATE_CA_TABLE_SIZE ) {
    // IONGATE_CA_MIN <= Ca <= IONGATE_CA_MAX
    p = c1[i]*p + c2[i];
  } else {
    // Do the correct calculation:
    double C1,C2;
    C1 = exp(-DT/tau(*Ca));
    C2 = (1.0-C1)*infty(*Ca);
    p = C1*p + C2;


    // IONGATE_CA_MAX < Ca
//    p = c1[IONGATE_CA_TABLE_SIZE-1]*p + c2[IONGATE_CA_TABLE_SIZE-1];
//    csimPrintf("CaGate_Yamada98::advance: [Ca] concentration (%g Mol) out of range (%g,%g)!\n",*Ca,IONGATE_CA_MIN,IONGATE_CA_MAX);
  }

  // calculate output
  P = ( k!=1 ) ? pow(p,k) : p;

  return 1;
}

int CaGate_Yamada98::addIncoming(Advancable *a)
{
  CaChannel_Yamada98 *c=dynamic_cast<CaChannel_Yamada98 *>(a);
  if ( c ) {
    Ca = &(c->Ca);
    return 0;
  } else {
    TheCsimError.add("CaGate_Yamada98::addIncoming: accept only CaChannel_Yamada98 as incoming objects (not a %s)!\n",a->className());
    return -1;
  }
}


void CaChannel_Yamada98::reset(void)
{
  Ca = 0;
  
  g = Gbar;
  for(int i=0;i<nGates;i++) {
    gates[i]->reset();
    g *= (gates[i]->P);
  }
}


int CaChannel_Yamada98::updateInternal(void)
{
  g = Gbar;
  for(int i=0;i<nGates;i++) {
    if ( gates[i]->updateInternal() < -1 ) return -1;
    g *= (gates[i]->P);
  }
  C1 = exp(-DT/Ts);
  return 0;
};

int CaChannel_Yamada98::advance(void)
{
  g = Gbar;
  for(int i=0;i<nGates;i++) {
    gates[i]->advance();
    g *= (gates[i]->P);
  }
  Ca = Ca*C1;
  return 1;
}

//==================================================================================

// rescale and calculate gating function time constant tau(v) (for variable Ts)
// if one of the two channel parameters gbar and tau are changed
// (because the IONGATE_TABLE is otherwise only calculated once during the very first call )
#ifndef _GUN_SOURCE
#define finite(x) (1)
#endif

# define UPDATE_INTERNAL_CONCGATE(_className_) \
int _className_::updateInternal(void) \
{ \
  g = Gbar; \
  for(int j=0;j<nGates;j++) { \
    if ( gates[j]->updateInternal() < -1 ) return -1; \
    g *= (gates[j]->P); \
 \
    /* rescale and calculate gating function time constant tau(v) (for variable Ts)*/ \
 \
    ConcIonGate *cgate;  \
    cgate = (ConcIonGate *)gates[j];  \
 \
    int i; double c; double *p1,*p2; \
    for(c=CONCIONGATE_CONC_MIN,i=0,p1=gates[j]->c1,p2=gates[j]->c2;i<CONCIONGATE_TABLE_SIZE;i++,c+=CONCIONGATE_CONC_INC,p1++,p2++) { \
      (*p1) = exp(-DT/(Ts*cgate->tau(c))); \
      (*p2) = (1.0-(*p1))*cgate->infty(c); \
      if ( !finite((*p1)) ) { \
         TheCsimError.add("_className_::updateInternal: There occurred undefined values (NaN or Inf) for C1!\n"); \
	 return -1; \
      } \
      if ( !finite((*p2)) ) { \
         TheCsimError.add("_className_::updateInternal: There occurred undefined values (NaN or Inf) for C2!\n"); \
	 return -1; \
      } \
    } \
  } \
  return 0; \
}

# define UPDATE_INTERNAL_VGATE(_className_) \
int _className_::updateInternal(void) \
{ \
  g = Gbar; \
  for(int j=0;j<nGates;j++) { \
    if ( gates[j]->updateInternal() < -1 ) return -1; \
    g *= (gates[j]->P); \
 \
    /* rescale and calculate gating function time constant tau(v) (for variable Ts)*/ \
 \
    VIonGate *vgate;  \
    vgate = (VIonGate *)gates[j];  \
 \
    int i; double v; double *p1,*p2; \
    for(v=VIONGATE_VM_MIN,i=0,p1=gates[j]->c1,p2=gates[j]->c2;i<VIONGATE_TABLE_SIZE;i++,v+=VIONGATE_VM_INC,p1++,p2++) { \
      (*p1) = exp(-DT/(Ts*vgate->tau(v))); \
      (*p2) = (1.0-(*p1))*vgate->infty(v); \
      if ( !finite((*p1)) ) { \
         TheCsimError.add("_className_::updateInternal: There occurred undefined values (NaN or Inf) for C1!\n"); \
	 return -1; \
      } \
      if ( !finite((*p2)) ) { \
         TheCsimError.add("_className_::updateInternal: There occurred undefined values (NaN or Inf) for C2!\n"); \
	 return -1; \
      } \
    } \
  } \
  return 0; \
}



UPDATE_INTERNAL_VGATE(AChannel_Korngreen02)

UPDATE_INTERNAL_VGATE(KChannel_Korngreen02)

UPDATE_INTERNAL_VGATE(NPChannel_McCormick02)

UPDATE_INTERNAL_VGATE(MChannel_Mainen96)

UPDATE_INTERNAL_VGATE(HChannel_Stuart98)

UPDATE_INTERNAL_VGATE(HVACAChannel_Brown93)

UPDATE_INTERNAL_VGATE(CALChannel_Destexhe98)

UPDATE_INTERNAL_CONCGATE(KCAChannel_Mainen96)

UPDATE_INTERNAL_VGATE(NAChannel_Traub91)

UPDATE_INTERNAL_VGATE(KDChannel_Traub91)

UPDATE_INTERNAL_VGATE(MChannel_Mainen96orig)





