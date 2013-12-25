#include <stdlib.h>
#include <math.h>
#include "conciongate.h"
#include "ionchannel.h"
#include "csimerror.h"


double *ConcIonGate::C1=0;
double *ConcIonGate::C2=0;


double ConcIonGate::pInfty(MembranePatchSimple *m) {

   MembranePatch *mp=(MembranePatch *)m;
   double *C; mp->buffers[ConcType]->getConc(&C);
   return infty(*C);
}


void ConcIonGate::ConnectToMembrane(MembranePatchSimple *m) {

   MembranePatch *mp=dynamic_cast<MembranePatch *>(m);
   if (mp) {
      if (mp->nBuffers < (ConcType+1)) {
         TheCsimError.add("ConcIonGate::ConnectToMembrane: IonBuffer %i not implemented!\n",ConcType);
	 return;
      }

      double *C; mp->buffers[ConcType]->getConc(&C);
      this->Conc = C;
      this->ConcRest = &(mp->buffers[ConcType]->ConcRest);
      this->ConcScale = &(mp->buffers[ConcType]->ConcScale);
   } else {
      TheCsimError.add("ConcIonGate::ConnectToMembrane: accept only MembranePatch as incoming objects!\n");
   }
}


int ConcIonGate::updateInternal(void) {
  // get the static table pointers and store them locally
  c1=getC1();
  c2=getC2();

  // allocate memory
  bool haveToInitTable = 0;

  if (!c1) { setC1(c1=(double *)malloc(CONCIONGATE_TABLE_SIZE*sizeof(double))); haveToInitTable = 1;}
  if (!c2) { setC2(c2=(double *)malloc(CONCIONGATE_TABLE_SIZE*sizeof(double))); haveToInitTable = 1;}

  //
  // set up the look up tables
  //
  if ( haveToInitTable ) {
    int i; double c; double *p1,*p2;
    for(c=CONCIONGATE_CONC_MIN,i=0,p1=c1,p2=c2;i<CONCIONGATE_TABLE_SIZE;i++,c+=CONCIONGATE_CONC_INC,p1++,p2++) {
      (*p1) = exp(-DT/tau(c));
      (*p2) = (1.0-(*p1))*infty(c);
#ifdef _GNU_SOURCE
      if ( !finite((*p1)) ) {
	TheCsimError.add("ConcIonGate::reset: There occurred undefined values (NaN or Inf) for C1!\n");
	return -1;
      }
      if ( !finite((*p2)) ) {
	TheCsimError.add("ConcIonGate::reset: There occurred undefined values (NaN or Inf) for C2!\n");
	return -1;
      }
#endif
    }
  }
  return 0;
}

void ConcIonGate::reset(void)
{
   if ( Conc!=0 ) {
//   if ( ( *Conc < CONCIONGATE_CONC_MIN ) || ( CONCIONGATE_CONC_MAX < *Conc ) ) {
//      TheCsimError.add("ConcIonGate::reset: [Conc] concentration (%g Mol) out of range (%g,%g)!\n",
//         			*Conc,CONCIONGATE_CONC_MIN,CONCIONGATE_CONC_MAX);
//      return;
//    }
    // set p to the 'resting value' at time t=0
    p = infty(*ConcRest);
  } else {
    TheCsimError.add("ConcIonGate::reset: IonGate not connected to any [Conc] concentration!\n");
    return;
  }

  // calculate output
  P = ( k!=1 ) ? pow(p,k) : p;
}

int ConcIonGate::advance(void)
{
  // do the table lookup
  int i=(int)((*Conc-CONCIONGATE_CONC_MIN)/CONCIONGATE_CONC_INC + 0.5);

  // exponential euler integratio step
  if ( i < 0 ) {
    // Do the correct calculation:
    double C1,C2;
    C1 = exp(-DT/tau(*Conc));
    C2 = (1.0-C1)*infty(*Conc);
    p = C1*p + C2;

    // Conc < CONCIONGATE_CONC_MIN
//    p = c1[0]*p + c2[0];
//    csimPrintf("ConcIonGate::advance: [Conc] concentration (%g Mol) out of range (%g,%g)!\n",*Conc,CONCIONGATE_CONC_MIN,CONCIONGATE_CONC_MAX);
  } else if ( i < CONCIONGATE_TABLE_SIZE ) {
    // CONCIONGATE_CONC_MIN <= Conc <= CONCIONGATE_CONC_MAX
    p = c1[i]*p + c2[i];

  } else {
    // Do the correct calculation:
    double C1,C2;
    C1 = exp(-DT/tau(*Conc));
    C2 = (1.0-C1)*infty(*Conc);
    p = C1*p + C2;

    // CONCIONGATE_CONC_MAX < Conc
//    p = c1[CONCIONGATE_TABLE_SIZE-1]*p + c2[CONCIONGATE_TABLE_SIZE-1];
//    csimPrintf("ConcIonGate::advance: [Conc] concentration (%g Mol) out of range (%g,%g)!\n",*Conc,CONCIONGATE_CONC_MIN,CONCIONGATE_CONC_MAX);
  }

  // calculate output
  P = ( k!=1 ) ? pow(p,k) : p;

  return 1;
}

