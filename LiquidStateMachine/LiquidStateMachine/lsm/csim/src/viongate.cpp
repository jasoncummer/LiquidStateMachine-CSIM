#include "viongate.h"
#include "ionchannel.h"
#include "csimerror.h"

double *VIonGate::C1=0;
double *VIonGate::C2=0;

int VIonGate::updateInternal(void) {
  // get the static table pointers and store them locally
  c1=getC1();
  c2=getC2();

  // printf("VIonGate::updateInternal\n");

  // allocate memory
  bool haveToInitTable = 0;
  if (!c1) { setC1(c1=(double *)malloc(VIONGATE_TABLE_SIZE*sizeof(double))); haveToInitTable = 1; }
  if (!c2) { setC2(c2=(double *)malloc(VIONGATE_TABLE_SIZE*sizeof(double))); haveToInitTable = 1; }

  if (dttable != DT) {haveToInitTable = 1;}

  //
  // set up the look up tables
  //
  if ( haveToInitTable ) {
    int i; double v; double *p1,*p2;

    if (nummethod == 0) {   
 
       // generate lookup table for exponential euler method
// printf("VIonGate::updateInternal Euler gate\n");

       for(v=VIONGATE_VM_MIN,i=0,p1=c1,p2=c2;i<VIONGATE_TABLE_SIZE;i++,v+=VIONGATE_VM_INC,p1++,p2++) {
         (*p1) = exp(-DT/tau(v));
         (*p2) = (1.0-(*p1))*infty(v);

#ifndef _WIN32
         if ( !finite((*p1)) ) {
      	   TheCsimError.add("VIonGate::updateInternal: There occurred undefined values (NaN or Inf) for C1!\n");
   	   return -1;
         }
         if ( !finite((*p2)) ) {
	   TheCsimError.add("VIonGate::updateInternal: There occurred undefined values (NaN or Inf) for C2!\n");
	   return -1;
         }
#endif
       }
   } else {

// printf("VIonGate::updateInternal Crank-Nicolson gate\n");
 
       // generate lookup table for Crank-Nicolson method

       // See: Methods in Neuronal Modeling: From Ions to Networks
       // edited by Christof Koch and Idan Segev
       // Chapter 14: "Numerical Methods for Neuronal Modeling" 
       // by Michael V. Mascagni and Arthur S. Sherman.
       for(v=VIONGATE_VM_MIN,i=0,p1=c1,p2=c2;i<VIONGATE_TABLE_SIZE;i++,v+=VIONGATE_VM_INC,p1++,p2++) {
         (*p1) = (1 - DT/2*(alpha(v) + beta(v))) / (1 + DT/2*(alpha(v) + beta(v)));
         (*p2) = DT*alpha(v)/(1+DT/2*(alpha(v) + beta(v)));

#ifndef _WIN32
         if ( !finite((*p1)) ) {
      	   TheCsimError.add("VIonGate::updateInternal: There occurred undefined values (NaN or Inf) for C1!\n");
   	   return -1;
         }
         if ( !finite((*p2)) ) {
	   TheCsimError.add("VIonGate::updateInternal: There occurred undefined values (NaN or Inf) for C2!\n");
	   return -1;
         }
#endif
       }

   } 
  }
  return 0;
}

void VIonGate::reset(void)
{
  if ( ( Vm!=0 ) && ( VmScale!=0 ) && ( Vresting!=0 ) ) {
    //    printf("VIonGate::reset\n");
    if ( ( *Vm < VIONGATE_VM_MIN ) || ( VIONGATE_VM_MAX < *Vm ) ) {
      TheCsimError.add("VIonGate::reset: Membrane voltage (%g Volt) out of range (%g,%g)!\n",
		       *Vm,VIONGATE_VM_MIN,VIONGATE_VM_MAX);
      return;
    }
    if ( ( *Vresting < VIONGATE_VM_MIN ) || ( VIONGATE_VM_MAX < *Vresting ) ) {
      TheCsimError.add("VIonGate::reset: Resting membrane voltage (%g Volt) out of range (%g,%g)!\n",
		       *Vresting,VIONGATE_VM_MIN,VIONGATE_VM_MAX);
      return;
    }
    // set p to the 'resting value' at time t=0

    // do the table lookup
    int i=(int)((*Vm-VIONGATE_VM_MIN)/VIONGATE_VM_INC + 0.5);
    p = c2[i]/(1.0-c1[i]);

    // p = infty(*Vm);

  } else {
    TheCsimError.add("VIonGate::reset: VIonGate not connected to any membrane voltage!\n");
    return;
  }
  // calculate output
  P = ( k!=1 ) ? pow(p,k) : p;
}

int VIonGate::advance(void)
{
  // do the table lookup
  int i=(int)((*Vm-VIONGATE_VM_MIN)/VIONGATE_VM_INC + 0.5);

  // exponential euler integration step
  if ( i < 0 ) {
    // Vm < VIONGATE_VM_MIN
    p = c1[0]*p + c2[0];
    // csimPrintf("VIonGate::advance: Membrane voltage (%g Volt) out of range (%g,%g)!\n",*Vm,VIONGATE_VM_MIN,VIONGATE_VM_MAX);
  } else if ( i < VIONGATE_TABLE_SIZE ) {
    // VIONGATE_VM_MIN <= Vm <= VIONGATE_VM_MAX
    p = c1[i]*p + c2[i];

  } else {
    // VIONGATE_VM_MAX < Vm
    p = c1[VIONGATE_TABLE_SIZE-1]*p + c2[VIONGATE_TABLE_SIZE-1];
    // csimPrintf("VIonGate::advance: Membrane voltage (%g Volt) out of range (%g,%g)!\n",*Vm,VIONGATE_VM_MIN,VIONGATE_VM_MAX);
  }

  // calculate output
  P = ( k!=1 ) ? pow(p,k) : p;

  return 1;
}

