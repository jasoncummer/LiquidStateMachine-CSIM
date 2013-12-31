/*! \file extoutlinearneuron.cpp
**  \brief Implementation of ExtOutLinearNeuron
*/

#ifdef _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif
#include "extoutlinearneuron.h"
#include "csimerror.h"

ExtOutLinearNeuron::ExtOutLinearNeuron(void)
{
  // attention: this is a bogus index
  // every ext neuron MUST have a valid index that has to be set!
  myIndex = -1;

  // per default we run in real time mode
  beReal = 1;
}

ExtOutLinearNeuron::~ExtOutLinearNeuron()
{
#ifdef _GNU_SOURCE
  if (beReal) {
    if (nSharedMemUse) {
      nSharedMemUse--;
      printf("stopping external I/O ... %i remaining\n",nSharedMemUse);
      munmap(sharedData,nMaxExtIO*sizeof(sharedData));
      close(memID);
    } //else
    //printf("WARNING: invalid destruction of external input %i!\n  => THIS SHOULD NEVER HAPPEN !!!\n",nSharedMemUse);
  }
#endif
}

void ExtOutLinearNeuron::reset(void)
{
  LinearNeuron::reset();

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

void ExtOutLinearNeuron::output(void)
{
  LinearNeuron::output();
#ifdef _GNU_SOURCE
  if (beReal) {
    sharedData[myIndex] = VmOut;
  }
#endif
}
