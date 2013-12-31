/*! \file extoutsigmoidalneuron.cpp
**  \brief Implementation of ExtOutSigmoidalNeuron
*/

#ifdef _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif
#include "extoutsigmoidalneuron.h"
#include "csimerror.h"

ExtOutSigmoidalNeuron::ExtOutSigmoidalNeuron(void)
{
  // attention: this is a bogus index
  // every ext neuron MUST have a valid index that has to be set!
  myIndex = -1;

  // per default we run in real time mode
  beReal = 1;
}

ExtOutSigmoidalNeuron::~ExtOutSigmoidalNeuron()
{
#ifdef _GNU_SOURCE
  if (beReal) {
    if (nSharedMemUse) {
      nSharedMemUse--;
      printf("stopping external I/O ... %i remaining\n",nSharedMemUse);
      munmap(sharedData,nMaxExtIO*sizeof(sharedData));
      close(memID);
    } //else
    //printf("WARNING: invalid destruction of external input!\n  => THIS SHOULD NEVER HAPPEN !!!\n");
  }
#endif
}

void ExtOutSigmoidalNeuron::reset(void)
{
  SigmoidalNeuron::reset();

#ifdef _GNU_SOURCE
  if (beReal) {
    if (!nSharedMemUse) {
      /* if no shared mem is used until now, we have to initialize the
	 shared mem sub-system ..... */

      printf("Detected usage of external I/O: initializing ...\n");
      if ((memID=open(rtMemFile, O_RDWR))<0) {
	TheCsimError.add("ERROR: can not open shared mem file ...\n       The external application must handle the creation and correct size of that file!\n"); return;
      }

      if (!(sharedData = (double *) mmap (0,nMaxExtIO*sizeof(sharedData),PROT_READ|PROT_WRITE,MAP_SHARED,memID,0))) {
	close(memID);
	TheCsimError.add("did not get mmap area ..."); return;
      }
    }

    // mark the use of shared mem
    nSharedMemUse++;
    // the workaround with 'nMaxExtIO' can be eliminated once we get the
    // data structure containing the number of external I/O ...
    if ( nSharedMemUse > nMaxExtIO ) {
      TheCsimError.add("ERROR: shared mem limit reached. Increase nMaxExtIO in globalvaribales.h\n"); return;
    }

    sharedData[myIndex] = Vm;
  }
#endif
}

void ExtOutSigmoidalNeuron::output(void)
{
  SigmoidalNeuron::output();
#ifdef _GNU_SOURCE
  if (beReal) {
    sharedData[myIndex] = VmOut;
  }
#endif
}
