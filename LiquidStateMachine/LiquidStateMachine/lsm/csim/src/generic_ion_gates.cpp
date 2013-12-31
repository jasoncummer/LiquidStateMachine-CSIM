#include <math.h>
#include "generic_ion_gates.h"
#include "ionchannel.h"
#include "csimerror.h"

int GVD_Gate::updateInternal(void) {
  // get the static table pointers and store them locally
  c1=getC1();
  c2=getC2();

  // allocate memory
  bool haveToInitTable = 1;
  if (!c1) { setC1(c1=(double *)malloc(VIONGATE_TABLE_SIZE*sizeof(double)));}
  if (!c2) { setC2(c2=(double *)malloc(VIONGATE_TABLE_SIZE*sizeof(double)));}

  //
  // set up the look up tables
  //
  if ( haveToInitTable ) {
    int i; double v; double *p1,*p2;
    for(v=VIONGATE_VM_MIN,i=0,p1=c1,p2=c2;i<VIONGATE_TABLE_SIZE;i++,v+=VIONGATE_VM_INC,p1++,p2++) {
      (*p1) = exp(-DT/tau(v));
      (*p2) = (1.0-(*p1))*infty(v);
#ifdef _GUN_SOURCE
      if ( !finite((*p1)) ) {
	TheCsimError.add("GVD_Gate::reset: There ocuured undefined values (NaN or Inf) for C1!\n");
	return -1;
      } 
      if ( !finite((*p2)) ) {
	TheCsimError.add("GVD_Gate::reset: There ocuured undefined values (NaN or Inf) for C2!\n");
	return -1;
      }
#endif
    }
  }
  return 0;
}


int GVD_cT_Gate::updateInternal(void) {
  // get the static table pointers and store them locally
  c1=getC1();
  c2=getC2();

  // allocate memory
  bool haveToInitTable = 1;
  if (!c1) { setC1(c1=(double *)malloc(VIONGATE_TABLE_SIZE*sizeof(double)));}
  if (!c2) { setC2(c2=(double *)malloc(VIONGATE_TABLE_SIZE*sizeof(double)));}
 
  //
  // set up the look up tables
  //
  if ( haveToInitTable ) {
    int i; double v; double *p1,*p2;
    for(v=VIONGATE_VM_MIN,i=0,p1=c1,p2=c2;i<VIONGATE_TABLE_SIZE;i++,v+=VIONGATE_VM_INC,p1++,p2++) {
      (*p1) = exp(-DT/tau(v));
      (*p2) = (1.0-(*p1))*infty(v);
#ifdef _GUN_SOURCE
      if ( !finite((*p1)) ) {
	TheCsimError.add("GVD_cT_Gate::reset: There ocuured undefined values (NaN or Inf) for C1!\n");
	return -1;
      }
      if ( !finite((*p2)) ) {
	TheCsimError.add("GVD_cT_Gate::reset: There ocuured undefined values (NaN or Inf) for C2!\n");
	return -1;
      }
#endif 
    }
  }
  return 0;
}

