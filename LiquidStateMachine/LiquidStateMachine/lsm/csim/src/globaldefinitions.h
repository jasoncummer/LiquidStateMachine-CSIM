/** \file  globaldefinitions.h
    \brief Definition of some global variables and unique ID's for all classes. 
*/


#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

// maximal synaptic delay in seconds
#define MAX_SYNDELAY 0.1f

//! 32 bit unsigned integers [on Pentium Linux PC]
typedef unsigned long uint32;

//! 16 bit unsigned integers [on Pentium Linux PC]
typedef unsigned short uint16;

//! This function just returns TheNetwork->t; i.e. the Simulation time
double simTime(void);

//! This function just returns TheNetwork->dt; i.e. the Simulation step size
double simDT(void);

//! This function just returns TheNetwork;
class Network;
Network *TheCurrentNetwork(void);

//! An alias for the current simulation time
#define SimulationTime (simTime())

//! An alias for the integration time step
#define DT (simDT())

//! Constant for spike driven simulation: after PSR_MULTIPLE_TAU*tau the psr is cut off.
#define PSR_MULTIPLE_TAU 5


//! This should assume a very large positive integer number
extern int MAXINTEGER;

//! Shared Memory constants and variables
// currently, these are defined in analogneuron.cpp

//! sort of a semaphore showing the use of shared mem
extern int nSharedMemUse; 

//! pointer to shared mem               
extern double* sharedData;

//! file ID of shared mem file               
extern int memID;

//! name of the shared mem file
extern const char* rtMemFile;

//! maximum external inputs and outputs
#define nMaxExtIO 1000 // (needed to avoid dynamic shMem allocation; change when no. of neurons is known


#ifdef MATLAB_MEX_FILE
#include <mex.h>
#define csimPrintf mexPrintf
#else
#include <stdio.h>
#define csimPrintf printf
#endif

#endif



