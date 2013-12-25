/*! \file network.cpp
**  \brief Implementation of Network
*/

// #define DEBUG_RT

#include "globaldefinitions.h"
#include "network.h"
#include "csimerror.h"
#include "spikingneuron.h"
#include "synapse.h"
#include "analogsynapse.h"
#include "spikingsynapse.h"
#include "teacher.h"
#include "recorder.h"
/* *********** BEGIN MICHAEL PFEIFFER ********* */
#include "readout.h"
#include "physicalmodel.h"
/* *********** END MICHAEL PFEIFFER ********* */
#include "randgen.h"
#include "csiminputclass.h"
#ifndef _WIN32
#include <sys/time.h>
#endif

#define max(A, B) ((A) > (B) ? (A) : (B))

#define CONN_INC  10000
#define ALLOC 20
#define FRAC_SYN               50
#define MIN_LENGTH_OF_FREELIST 100


#define neuron(i) (allNeuronList.elements[(i)])
#define nNeurons (allNeuronList.n)
   
#define notTeached(i) (notTeachedList.elements[(i)])
#define nNotTeached  (notTeachedList.n)

#define teached(i) (teachedList.elements[(i)])
#define nTeached   (teachedList.n)

#define synapse(i) (synapseList.elements[(i)])
#define nSynapses  (synapseList.n)

#define teacher(i) (teacherList.elements[(i)])
#define nTeachers  (teacherList.n)

#define recorder(i) (recorderList.elements[(i)])
#define nRecorders  (recorderList.n)

/* ******** BEGIN MICHAEL PFEIFFER ************ */
#define readouts(i) (readoutList.elements[(i)])
#define nReadouts  (readoutList.n)

#define models(i) (modelList.elements[(i)])
#define nModels  (modelList.n)
/* ********** END MICHAEL PFEIFFER ************ */

#define object(i) (objectList.elements[(i)])
#define nObjects  (objectList.n)


/****************************************************************************
 ** Constructor/Destructor
 ****************************************************************************/

Network::Network() {

  // fields/parameters of Network
  t            = 0.0;
  step         = 0;
  randSeed     = 66403;
  verboseLevel = 1;
  dt           = 1e-4;
  spikeOutput  = 1;
  nThreads     = 1;

  // variables needed for the export/import commands
  nDst = lDst = 0; dst = 0;
  nSrc = lSrc = 0; src = 0;
  nRecMem = lRecMem = 0; RecMem=0;
  /* ********** BEGIN MICHAEL PFEIFFER ************ */
  nReadoutMem = lReadoutMem = 0; ReadoutMem = 0;
  nModelMem = lModelMem = 0; ModelMem = 0;
  /* ********** END MICHAEL PFEIFFER ************ */

  // list for event-driven part of simulation
  delayList = 0;               

  // internal flags
  isReset = 0;
  dtChanged = 1; 
  fieldsOfSomeObjectChanged = 1;

}


Network::~Network() {
  //
  // Delete all objects. Hence objects like ion channels do not have
  // to be deleted by their parent neurons.
  //
  objectList.destroy();
  if (dst)    free(dst); dst = 0;
  if (src)    free(src); src = 0;
  if (RecMem) free(RecMem); RecMem = 0;
  /* ********** BEGIN MICHAEL PFEIFFER ************ */
  if (ReadoutMem) free(ReadoutMem); ReadoutMem = 0;
  if (ModelMem) free(ModelMem); ModelMem = 0;
  /* ********** END MICHAEL PFEIFFER ************ */

  allocList.destroy_arrays();

  if (delayList) free(delayList); delayList = 0;
}

/****************************************************************************
 ** Methods dealing with setting/getting field values
 ****************************************************************************/

int Network::setField(char *fieldName, double v)
{
  if ( 0 == strcmp(fieldName,"dt") ) {
    dt = v; dtChanged = 1;
  } else if ( 0 == strcmp(fieldName,"verboseLevel") ) {
    verboseLevel = (int)v; 
  } else if ( 0 == strcmp(fieldName,"randSeed") ) {
    randSeed = (int)v;
  } else if ( 0 == strcmp(fieldName,"spikeOutput") ) {
    spikeOutput = (int)v;
  } else if ( 0 == strcmp(fieldName,"nThreads") ) {
    nThreads = (int)v;
  } else {
    TheCsimError.add("Network::setField: Unknown fieldName %s!\n",fieldName); return -1;
  }
  return 0;
}

int Network::getField(char *fieldName, double *v)
{
  if ( 0 == strcmp(fieldName,"dt") ) {
    *v = dt;
  } else if ( 0 == strcmp(fieldName,"verboseLevel") ) {
    *v = verboseLevel;
  } else if ( 0 == strcmp(fieldName,"randSeed") ) {
    *v = randSeed;
  } else if ( 0 == strcmp(fieldName,"spikeOutput") ) {
    *v = spikeOutput;
  } else if ( 0 == strcmp(fieldName,"nThreads") ) {
    *v = nThreads;
  } else if ( 0 == strcmp(fieldName,"t") ) {
    *v = t;
  } else {
    TheCsimError.add("Network::getField: Unknown fieldName %s!\n",fieldName); return -1;
  }
  return 0;
}

void Network::printFields(void) {
  csimPrintf("          dt = %g sec\n",dt);
  csimPrintf("verboseLevel = %i\n",verboseLevel);
  csimPrintf("    randSeed = %i\n",randSeed);
  csimPrintf(" spikeOutput = %i\n",spikeOutput);
  csimPrintf("           t : %g sec\n",t);
  csimPrintf("        step : %i\n",step);
  csimPrintf("    nThreads : %i (for future use!)\n",nThreads);
  
}

int Network::getFieldArraySize(void) {
  return 5;
}

double *Network::getFieldArray(void) {
  double *p=(double *)malloc(getFieldArraySize()*sizeof(double));
  p[0] = dt;
  p[1] = verboseLevel;
  p[2] = randSeed;
  p[3] = spikeOutput;
  p[4] = nThreads;
  return p;
}

void Network::setFieldArray(double *p) {
  dt           = p[0];
  verboseLevel = (int)(p[1]);
  randSeed     = (int)(p[2]);
  spikeOutput  = (int)(p[3]);
  nThreads     = (int)(p[4]);
}

/****************************************************************************
 ** Adding and connecting objects
 ****************************************************************************/

int Network::addNewObject(Advancable *a)
{
  // Add a to the list of all objects for further access via indices.
  objectList.add(a);

  // if a is one of Neuron, Synapse, Teacher, or csimRecorder we insert them in extra lists
  if (dynamic_cast<Neuron   *>(a)) { allNeuronList.add((Neuron *)a); notTeachedList.add((Forceable *)a); }
  else if (dynamic_cast<Synapse  *>(a)) { synapseList.add((Synapse *)a);   }
  else if (dynamic_cast<Teacher  *>(a)) { teacherList.add((Teacher *)a); }
  else if (dynamic_cast<csimRecorder *>(a)) { recorderList.add((csimRecorder *)a); }
  /* **************** BEGIN MICHAEL PFEIFFER **************** */
  else if (dynamic_cast<Readout *>(a)) { readoutList.add((Readout *)a); }
  else if (dynamic_cast<Preprocessor *>(a)) { preprocessorList.add((Preprocessor *)a); }
  else if (dynamic_cast<Algorithm *>(a)) { algorithmList.add((Algorithm *)a); }
  else if (dynamic_cast<PhysicalModel *>(a)) { modelList.add((PhysicalModel *)a); }
  /* **************** END MICHAEL PFEIFFER **************** */

  // return the index of the object in the objectList.
  return (a->Id=nObjects-1);
}

Advancable *Network::getObject(uint32 i)
{
  if ( i <= nObjects ) {
    return object(i);
  } else
    { TheCsimError.add("Network::getAdvancable: invalid object index (%i).\n",i); return 0; }
}

// connect two objects
int Network::connect(Advancable *destination, Advancable *source)
{
  bool e1=(source->addOutgoing(destination) < 0 );
  bool e2=(destination->addIncoming(source) < 0 );

  if ( e1 || e2 ) {
    TheCsimError.add("Network::connect: error connection %s %i to %s %i\n",
                     source->className(),source->getId(),destination->className(),destination->getId());
    return -1;
  }

  return 0;
}

// connect two objects (by index)
int Network::connect(uint32 destination, uint32 source)
{
  if ( (destination < nObjects) &&  ( source < nObjects) ) {
    if ( connect(object(destination),object(source)) > -1 ) {
      // store destination and source indices
      if ( ++nDst > lDst )
        dst = (uint32 *)realloc(dst,(lDst += CONN_INC)*sizeof(uint32));
      if ( ++nSrc > lSrc )
        src = (uint32 *)realloc(src,(lSrc += CONN_INC)*sizeof(uint32));
      dst[nDst-1] = destination;
      src[nSrc-1] = source;
    }
  } else {
    TheCsimError.add("Network::connect: invalid object index (dst=%i, src=%i)\n",
                     object(destination)->getId(),object(source)->getId());
    return -1;
  }
  return 0;
}

// connect many to one (convergence)
int Network::connect(uint32 dst, uint32 *src, uint32 n)
{
  for(uint32 i=0;i<n;i++)
    if ( connect(dst,src[i]) < 0 ) return -1;
  return 0;
}

// connect one to may (divergence)
int Network::connect(uint32 *dst, uint32 src, uint32 n)
{
  for(uint32 i=0;i<n;i++)
    if ( connect(dst[i],src) < 0 ) return -1;
  return 0;
}

// connect one to one (src_i -> dst_i : i=1,...,n)
int Network::connect(uint32 *dst, uint32 *src, uint32 n)
{
  for(uint32 i=0;i<n;i++)
    if ( connect(dst[i],src[i]) < 0 ) return -1;

  return 0;
}

// connect one to one via another
int Network::connect(uint32 *postIdx, uint32 *preIdx, uint32 *synIdx , uint32 n)
{
  for (unsigned i=0; i<n; i++) {
    if ( connect(synIdx[i],preIdx[i]) < 0  ) return -1;
    if ( connect(postIdx[i],synIdx[i]) < 0 ) return -1;
  }
  return 0;
}

/* ************************* BEGIN MICHAEL PFEIFFER *********************** */

// connect fields to record to a Recorder or Readout or Physical Model
int Network::connect(uint32 recorderIdx, uint32 *objIdx, uint32 n, char *fieldName)
{
  // Try to connect recorder
  csimRecorder *r=dynamic_cast<csimRecorder *>(getObject(recorderIdx));

  if ( r ) {
    for (unsigned i=0; i<n; i++) {
      if ( r->addFieldToRecord(getObject(objIdx[i]),fieldName) < 0 ) {
	TheCsimError.add("Network::recorderConnect: "
			 "can not record field '%s' of object %i = objIdx[%i].\n",fieldName,objIdx[i],i); return -1;
      }
    }
    
    /** save the call for a potential csim('export') command later on we
     ** do this in a very C like way */
    
    int l,sz = sizeof(uint32)*(2+n)+(l=(strlen(fieldName)+1));
    if ( (nRecMem + sz) > lRecMem ) {
      RecMem = (char *)realloc(RecMem,(lRecMem += max(5*sz,2048)));
      pRecMem = RecMem + nRecMem;
    }
    
    memcpy(pRecMem,&recorderIdx,sizeof(uint32));       pRecMem += sizeof(uint32);
    memcpy(pRecMem,&n,sizeof(uint32));                 pRecMem += sizeof(uint32);
    memcpy(pRecMem,objIdx,sizeof(uint32)*n);           pRecMem += sizeof(uint32)*n;
    memcpy(pRecMem,fieldName,l);                       pRecMem += l;
    
    nRecMem += sz;
    
    return 0;
  }

  
  // Try to connect to readout
  Readout *ro = dynamic_cast<Readout *>(getObject(recorderIdx));
  
  if ( ro ) {
    for (unsigned i=0; i<n; i++) {
      if ( ro->addInputField(getObject(objIdx[i]),fieldName) < 0 ) {
	TheCsimError.add("Network::readoutConnect: "
			 "can not record field '%s' of object %i = objIdx[%i].\n",fieldName,objIdx[i],i); return -1;
      }
    }
    
    /** save the call for a potential csim('export') command later on we
     ** do this in a very C like way */
    
    int l,sz = sizeof(uint32)*(2+n)+(l=(strlen(fieldName)+1));
    if ( (nReadoutMem + sz) > lReadoutMem ) {
      ReadoutMem = (char *)realloc(ReadoutMem,(lReadoutMem += max(5*sz,2048)));
      pReadoutMem = ReadoutMem + nReadoutMem;
    }
    
    memcpy(pReadoutMem,&recorderIdx,sizeof(uint32));       pReadoutMem += sizeof(uint32);
    memcpy(pReadoutMem,&n,sizeof(uint32));                 pReadoutMem += sizeof(uint32);
    memcpy(pReadoutMem,objIdx,sizeof(uint32)*n);           pReadoutMem += sizeof(uint32)*n;
    memcpy(pReadoutMem,fieldName,l);                       pReadoutMem += l;
    
    nReadoutMem += sz;
    
    return 0;
  }

  // Try to connect to input of physical model
  PhysicalModel *phm = dynamic_cast<PhysicalModel *>(getObject(recorderIdx));
  
  if ( phm ) {
    for (unsigned i=0; i<n; i++) {
      if ( phm->addInput(getObject(objIdx[i]),fieldName) < 0 ) {
	TheCsimError.add("Network::modelConnect: "
			 "can not connect to input '%s' of object %i = objIdx[%i].\n",fieldName,objIdx[i],recorderIdx); return -1;
      }
    }
    
    /** save the call for a potential csim('export') command later on we
     ** do this in a very C like way */
    
    int l,sz = sizeof(uint32)*(2+n)+(l=(strlen(fieldName)+1));
    if ( (nModelMem + sz) > lModelMem ) {
      ModelMem = (char *)realloc(ModelMem,(lModelMem += max(5*sz,2048)));
      pModelMem = ModelMem + nModelMem;
    }
    
    memcpy(pModelMem,&recorderIdx,sizeof(uint32));       pModelMem += sizeof(uint32);
    memcpy(pModelMem,&n,sizeof(uint32));                 pModelMem += sizeof(uint32);
    memcpy(pModelMem,objIdx,sizeof(uint32)*n);           pModelMem += sizeof(uint32)*n;
    memcpy(pModelMem,fieldName,l);                       pModelMem += l;
    
    nModelMem += sz;
    
    return 0;
  }


  // Try to connect from outputs of physical models to another object
  phm = dynamic_cast<PhysicalModel *>(getObject(objIdx[0]));
  
  if ( phm ) {
    if (recorderIdx >= nObjects) {
      TheCsimError.add("Network::modelConnect: "
		       "invalid index %i for destination.\n",recorderIdx); return -1;
    }

    for (unsigned i=0; i<n; i++) {
      if (i>0)
	phm = dynamic_cast<PhysicalModel *>(getObject(objIdx[i]));
      if (phm) {
	if ( phm->addOutput(getObject(recorderIdx),fieldName) < 0 ) {
	  TheCsimError.add("Network::modelConnect: "
			   "can not output field '%s' of model %i = objIdx[%i].\n",fieldName,objIdx[i],i); return -1;
	}
	if (getObject(recorderIdx)->addIncoming(phm) < 0) {
	  TheCsimError.add("Network::modelConnect: error connection %s %i to %s %i\n",
			   phm->className(),objIdx[i],getObject(recorderIdx)->className(),recorderIdx);
	  return -1;
	}
      }
      else {
	TheCsimError.add("Network::modelConnect: Object %i = objIdx[%i] is not a valid model\n", objIdx[i],i); return -1; 
      }
    }
    
    /** save the call for a potential csim('export') command later on we
     ** do this in a very C like way */
    
    int l,sz = sizeof(uint32)*(2+n)+(l=(strlen(fieldName)+1));
    if ( (nModelMem + sz) > lModelMem ) {
      ModelMem = (char *)realloc(ModelMem,(lModelMem += max(5*sz,2048)));
      pModelMem = ModelMem + nModelMem;
    }
    
    memcpy(pModelMem,&recorderIdx,sizeof(uint32));       pModelMem += sizeof(uint32);
    memcpy(pModelMem,&n,sizeof(uint32));                 pModelMem += sizeof(uint32);
    memcpy(pModelMem,objIdx,sizeof(uint32)*n);           pModelMem += sizeof(uint32)*n;
    memcpy(pModelMem,fieldName,l);                       pModelMem += l;
    
    nModelMem += sz;
    
    return 0;
  }

  TheCsimError.add("Network::recorderConnect: no valid recorder, readout or physical model specified!\n"); return -1; 

}

/* ************************* END MICHAEL PFEIFFER *********************** */


/*******************************************************************************
 ** Teacher related
 *******************************************************************************/

int Network::moveToTeached(Forceable *f)
{
  // find object in notTeachedList
  unsigned long j = 0;
  while ( j<nNotTeached && f!=notTeached(j) ) j++;

  if ( j < nNotTeached ) {
    // move object from notTeachedList to teachedList
    // 1.) remove from notTeachedList by moving the last object to index j and decrementing n
    notTeached(j) = notTeached(nNotTeached-1); nNotTeached--;

    // 2.) add to teachedList
    teachedList.add(f);
  } else {
    TheCsimError.add("Network::connect: object %s %i not yet added to network or already taught by another teacher.\n",
                     f->className(),f->getId());
    return -1;

  }
  return 0;
}

/*********************************************************************************
 ** Upate, reset and simulate the Network
 ********************************************************************************/

int Network::updateObjects(void) {
  // befor we do anything else we have to make all objects self-consistend
  if ( fieldsOfSomeObjectChanged || dtChanged ) {
    for(long unsigned i=0; i<nObjects; i++)
      if ( object(i)->fieldChangeNotify(dtChanged) < 0 ) {
        TheCsimError.add("Network::upateObjects: Failed to update object %i (%s)\n",
                         i, object(i)->className());
        return -1; 
      }
    fieldsOfSomeObjectChanged = dtChanged = 0;
  }
  return 0;
}


int Network::reset()
{
  unsigned long i = 0;

  if ( updateObjects() < 0 ) 
    return -1;

  rseed(randSeed);

  // do the actual reset to t=0
  t    = 0.0;
  step = 0;
  for(i=0; i<nObjects; i++) {
    object(i)->reset();
  }


  // deal with event-driven memory management (Christian is the only who knows how it works ;-)
  if (isReset) {
    // empty delayList and reset memory management
    memset(delayList, 0, ldelayList * sizeof(event*));
    delayIdx = 0;
    freeList = allocList.elements[0];
    freeIdx = 0;
    allocIdx = 1;
    recycledEvent = 0;
  } else {
    // we have one list of fixed length to store all events depending on their delay
    ldelayList = (int)(MAX_SYNDELAY/this->dt+0.5)+1;
    delayList  = (event **)calloc(ldelayList, sizeof(event*));
    delayIdx   = 0;

    // build event lists (list of synapses who will receive a spike after a certain delay)
    lfreeList = MIN_LENGTH_OF_FREELIST + synapseList.n/FRAC_SYN;
    freeList = new event[lfreeList];

    // we do our memory management by ourselfs to avoid unnecessary malloc/new calls
    allocList.add(freeList);
    freeIdx = 0;
    allocIdx = 1;
    recycledEvent = 0;

    // finally say that we did a reset
    isReset = 1;
  }

  // Build list of active synapses and copy all analog synapses.
  // SpikingSynapses are inactive by default.
  activeSyn.n = 0;
  AnalogSynapse *as;
  for (i=0; i<nSynapses; i++)
    if ((as = dynamic_cast<AnalogSynapse *>(synapse(i))))
      activeSyn.add(as);

  return 0;
}

int Network::assignInputs(csimInputChannel *input, int nInputs)
{
  int nT=0; // number of teachers
  csimInputClass *p;
  for(int i=0;i<nInputs;i++) {
    for(int j=0;j<input[i].nIdx;j++) {
      p=dynamic_cast<csimInputClass *>(object(input[i].idx[j]));
      if ( p ) {
        if ( p->addInputChannel(input+i) < 0 ) { 
          TheCsimError.add("Cound not assign Input(%i) to %s (idx=%i)",
                           i,object(input[i].idx[j])->className(),input[i].idx[j]); 
          return -1;
        }
      } else {
        TheCsimError.add("Object (%s) referenced by Input(%i).idx=%i does not accept inputs",
                         object(input[i].idx[j])->className(),i,input[i].idx[j]);
        return -1;
      }
      if ( dynamic_cast<Teacher *>(object(input[i].idx[j])) )
        nT++;
    }
  }
  return nT;
}

int Network::unAssignInputs(csimInputChannel *input, int nInputs)
{
  csimInputClass *p;
  for(int i=0;i<nInputs;i++) {
    for(int j=0;j<input[i].nIdx;j++) {
      p=dynamic_cast<csimInputClass *>(object(input[i].idx[j]));
      if ( p ) p->clearInputChannels();
    }
  }
  return 0;
}


#ifndef _WIN32
long getCurrentTime(void)
  /* returns current time in MICROSECONDS */
{
  struct timeval tv;

  gettimeofday(&tv,NULL);
  return(tv.tv_sec*1000000+tv.tv_usec);
}
#endif

void Network::ScheduleSpike(SpikingSynapse * s, pSpikeHandler h, double delay)
{

  // calculate index where to insert the synapse into the delayList
  int idx = delayIdx + (int) (delay / this->dt + 0.5);
  if (idx >= ldelayList)
    idx -= ldelayList;

  // Event memory management: get a event slot
  event *e;
  if (recycledEvent != 0) {
    // get a recycled  event slot
    e = recycledEvent;
    recycledEvent = recycledEvent->next;
  } else if (freeIdx < lfreeList) {
    // get slot from the current (allocIdx) pre-allocated memory chunk
    e = &(freeList[freeIdx++]);
  } else if (allocIdx < allocList.n) {
    // current allocIdx) pre-allocated memory chunk used up: go to
    // next chunk
    freeList = allocList.elements[allocIdx++];
    e = &(freeList[0]);
    freeIdx = 1;
  } else {
    // no more chunks available: alloc a new one
    freeList = new event[lfreeList];
    allocList.add (freeList);
    ++allocIdx;
    e = &(freeList[0]);
    freeIdx = 1;
  }
  
  // insert the event into the list of event at position delayIdx of
  // the delayList
  e->s = s;
  e->h = h;
  e->next = delayList[idx];
  delayList[idx] = e;
}

 
int Network::simulate(unsigned long nSteps, csimInputChannel *inputSignal, int ni, csimInputChannel *teacherSignal, int nt) {
  
  //  printf("Network::simulate: enter\n");

  int teach=0;

  unsigned long i;

  // update internals of all objects
  if ( updateObjects() < 0 )
    return -1;

  //  printf("Network::simulate: update\n");

  // reset if necessary
  if ( !isReset ) reset();

  //  printf("Network::simulate: reset\n");

  // assign the inputs
  if ( (teach=assignInputs(inputSignal, ni)) < 0 ) {
    TheCsimError.add("Network::simulate: Error assigning inputs!\n"); return -1;
  }

  //  printf("Network::simulate: assign\n");


  // assign input for the teachers
  if ( assignInputs(teacherSignal, nt) < 0 ) {
    TheCsimError.add("Network::simulate: Error assigning teacher inputs!\n"); return -1;
  }

  //   printf("after assign\n");

  // There are active Teacher if some input channel points to a
  // Teacher or if there are explicit teaching signals
  teach = (teach > 0) || (nt > 0);

#ifndef _WIN32
  if (nSharedMemUse) {
    lastSyncTime = getCurrentTime();
    DT_us = (long)(dt*1000000);    // calculate our DT in microseconds
    printf("  Running in RT mode (no. %i) ...\n",nSharedMemUse);
  }
#endif

  for (unsigned long s = 0; s<nSteps; s++) {

    t+=dt; step++; // increment virtual time

    // advance all neurons without a teacher: i.e. nextstate() and output() gets called.
    for (i=0; i<nNotTeached; i++)
      notTeached(i)->advance();

    if ( teach ) {

      // the teacher calls nextstate(), force() and output() for all its neurons
      for (i=0; i<nTeachers; i++)
        teacher(i)->advance();
    } else {

      // if there are no teachers we have to call advance() of all teached neurons
      for (i=0; i<nTeached; i++)
        teached(i)->advance();
    }

    // process the delay list
    event *ev = delayList[delayIdx];
    if (ev != 0) {
      // Go through all the events at this time and add synapse to
      // acitve list if it is not there. This is determined within
      // preSpikeHit().
      // if ((ev->s->p >= 1.0 || ev->s->p > unirnd()) && (ev->s->preSpikeHit())) {
      if ( ((ev->s)->*(ev->h))() ) {
        activeSyn.add(ev->s);
      }
      while (ev->next != 0) {
        ev = ev->next;
        //if ((ev->s->p >= 1.0 || ev->s->p > unirnd()) && (ev->s->preSpikeHit())) {
        if ( ((ev->s)->*(ev->h))() ) {
          activeSyn.add(ev->s);
        }
      }

      // Event memory management: put the event slots back to the
      // recycled list.
      ev->next = recycledEvent;
      recycledEvent = delayList[delayIdx];

      // Mark this slot of the delayList as processed
      delayList[delayIdx] = 0;
    }

    // increase delayIdx with loop around
    if (++delayIdx >= ldelayList) delayIdx = 0;

    // ----- sync ----

    // advance active synapses
    i = 0;
    while (i<activeSyn.n) {
      if (!activeSyn.elements[i]->advance()) {
        // remove a synapse from the active list if its PSR is very close to zero
        activeSyn.elements[i] = activeSyn.elements[--(activeSyn.n)];
      } else
        // advance to next synapse
        i++;
    }

    /* *********** BEGIN MICHAEL PFEIFFER ********* */
    // advance readout elements
    for (i=0; i<nReadouts; i++)
      readouts(i)->advance();

    // advance physical models
    for (i=0; i<nModels; i++)
      models(i)->advance();
    /* *********** END MICHAEL PFEIFFER ********* */
    

    // advance recording elements
    for (i=0; i<nRecorders; i++)
      recorder(i)->advance();

#ifndef _WIN32
    //
    // here comes the "real time" stuff
    //
    if ( nSharedMemUse ) {
      long currTime, diffTime;
      /* if we're in real time mode (i.e. we receive REAL sensor data
         from our robot), we have to wait or to complain */

      /* REMARK: i don't use any sleep/usleep/nanosleep since
         these do not work reliably unless you use at
         least a hard-rt kernel */

      /* if time remains we wait ... */
      currTime = getCurrentTime();
      diffTime = currTime-lastSyncTime;
#ifdef DEBUG_RT
      csimPrintf("RT: Time (last/curr) [us]: %i / %i\n",lastSyncTime,currTime);
      csimPrintf("    Simulation time [us]: %i / %i\n",DT_us,diffTime);
#endif
      if (diffTime<DT_us) {
#ifdef DEBUG_RT
        csimPrintf("    Simulation time [us]: %i / %i\n",DT_us,diffTime);
#endif
        while((getCurrentTime()-lastSyncTime)<DT_us) {
          /* do ... nothing. just wait ... */
        }
        lastSyncTime += DT_us; /* getCurrentTime(); */
      } else {
        lastSyncTime = getCurrentTime(); /* getCurrentTime(); */
#ifdef DEBUG_RT
        csimPrintf("WARNING: simulation time SLOWER than real time: %i us !!!!!\n",diffTime-DT_us);
#endif
      }
#ifdef DEBUG_RT
      csimPrintf("    difference after waiting [us]: %i\n",getCurrentTime()-lastSyncTime);
#endif
      /* now save the new time as reference */
      //      lastSyncTime += DT_us; /* getCurrentTime(); */
    }
#endif

  }

  // allocList.destroy();
  // if (delayList) free(delayList); delayList = 0;
  //    printf("%g\n",t);

  if ( unAssignInputs(inputSignal, ni) < 0 ) {
    TheCsimError.add("Network::simulate: Error UNassigning inputs!\n"); return -1;
  }

  // assign input for the teachers
  if ( unAssignInputs(teacherSignal, nt) < 0 ) {
    TheCsimError.add("Network::simulate: Error UNassigning teacher inputs!\n"); return -1;
  }

  // printf("Network::simulate: leave\n");

  return 0;
}


unsigned long Network::totalSpikeCount(void)
{
  unsigned long spkCnt = 0;
  SpikingNeuron *n;

  for(unsigned i=0;i<nNeurons;i++)
    if ( (n=dynamic_cast<SpikingNeuron *>(neuron(i))) )
      spkCnt += (n->nSpikes());

  return spkCnt;
}

void Network::getSpikes(uint32 *idx, double *times)
{
  SpikingNeuron *n;

  for(unsigned i=0;i<nNeurons;i++)
    if ( (n=dynamic_cast<SpikingNeuron *>(neuron(i))) )
      for(int s=0;s<n->nSpikes();s++) {
        *times++=n->spikeTime(s);
        *idx++  =n->getId();
      }
}


int Network::getNumOf(char *cn) {
  int l=strlen(cn);
  if ( 0 == strncmp(cn,"Recorder",l) ) return nRecorders;
  else  if ( 0 == strncmp(cn,"Neuron",l) )   return nNeurons;
  else  if ( 0 == strncmp(cn,"Synapse",l) )  return nSynapses;
  else  if ( 0 == strncmp(cn,"Object",l) )   return nObjects;
  else  if ( 0 == strncmp(cn,"Teacher",l) )  return nTeachers;
  else {
    TheCsimError.add("Network::getNumOf: unknown class (%s)\n",cn); return -1;
  }
}

int Network::setParameter(uint32 *idx, int nIdx, char *paramName, double *value, int m, int n) {

  int i;

  if (nIdx<1 || idx[0] >= nObjects) {
    TheCsimError.add("Network::setParameter: invalid indices (nIdx=%i, idx[0]=%i, nObjects=%i).\n", nIdx, idx[0], nObjects);
    return -1;
  }
  int c = object(idx[0])->classId();

  int paramId = object(idx[0])->getFieldId(paramName);
  if (paramId < 0) {
    TheCsimError.add("Network::setParameter: Unknown parameter name '%s'.\n", paramName);
    return -1;
  }

  for (i=1; i<nIdx; i++)
    if (object(idx[i])->classId() != c) {
      TheCsimError.add("Network::setParameter: trying to set parameters of different classes.\n");
      return -1;
    }

  if ( m != object(idx[0])->getFieldSizeById((char *)object(idx[0]),paramId) ) {
    TheCsimError.add("Network::setParameter: field size of %s does not match (%i != %i).\n",
                     paramName, m, object(idx[0])->getFieldSizeById((char *)object(idx[0]),paramId));
    return -1;
  }

  Advancable *o;
  if ( n==1 ) {
    for (i=0; i<nIdx; i++) {
      o=object(idx[i]);
      o->setFieldById((char *)o,paramId, value);
    }
  } else if ( n==nIdx  ) {
    for (i=0; i<nIdx; i++) {
      o=object(idx[i]);
      o->setFieldById((char *)o,paramId, value+i*m);
    }
  } else {
    TheCsimError.add("Network::setParameter: invalid n=%i must be 1 or equal nIdx=%i.\n", n, nIdx);
    return -1;
  }

  fieldsOfSomeObjectChanged = 1;

  return 0;
}

int Network::getParameter(uint32 *idx, int n, char *paramName, double **value, int *m) {

  if (n<1 || idx[0] >= nObjects) {
    TheCsimError.add("Network::getParameter: invalid indices (n=%i, idx[0]=%i, nObjects=%i).\n", n, idx[0], nObjects);
    return -1;
  }
  int c = object(idx[0])->classId();

  int paramId = object(idx[0])->getFieldId(paramName);
  if (paramId < 0) {
    TheCsimError.add("Network::getParameter: Unknown parameter name '%s'.\n", paramName);
    return -1;
  }

  int i;
  for(i=1; i<n; i++) {
    if (object(idx[i])->classId() != c) {
      TheCsimError.add("Network::getParameter: trying to get parameters of different classes.\n");
      return -1;
    }
  }

  int mm = *m = object(idx[0])->getFieldSizeById((char *)object(idx[0]),paramId);
  *value=(double *)malloc(mm*n*sizeof(double));

  for (i=0; i<n; i++)
    object(idx[i])->getFieldById((char *)object(idx[i]),paramId,(*value)+i*mm);

  return 0;
}

int Network::listObjects(bool fields) {
  if ( fields ) {
    for(long unsigned i=0; i<nObjects; i++)
      object(i)->printFields((char *)object(i));
  } else {
    for(long unsigned i=0; i<nObjects; i++)
      csimPrintf("%5i : %s\n",i,object(i)->className());
  }
  return 0;
}

/* ******************* BEGIN MICHAEL PFEIFFER ******************** */
//! Import preprocessors or algorithm
int Network::importObject(uint32 *idx, int nIdx, double *value, int n) {

  int i;

  if (nIdx<1 || idx[0] >= nObjects) {
    TheCsimError.add("Network::importObject: invalid indices (nIdx=%i, idx[0]=%i, nObjects=%i).\n", nIdx, idx[0], nObjects);
    return -1;
  }

  Preprocessor *pre;
  Algorithm *alg;

  for (i=0; i<nIdx; i++) {
    // Import preprocessor
    if ( (pre = dynamic_cast<Preprocessor *>(object(idx[i]))) )
      pre->importRepresentation(value, n);

    // Import algorithm
    else if ( (alg = dynamic_cast<Algorithm *>(object(idx[i]))) )
      alg->importRepresentation(value, n);

    else {
      // Invalid object requested
      TheCsimError.add("Network::importObject: can only import preprocessors and algorithms.\n");
      return -1;
    }
  }

  fieldsOfSomeObjectChanged = 1;

  return 0;
}

//! Export preprocessors or algorithm
int Network::exportObject(uint32 *idx, int nIdx, double ***value, int **m) {
  if (nIdx<1 || idx[0] >= nObjects) {
    TheCsimError.add("Network::exportObject: invalid indices (nIdx=%i, idx[0]=%i, nObjects=%i).\n", nIdx, idx[0], nObjects);
    return -1;
  }

  int i;
  Preprocessor *pre;
  Algorithm *alg;

  // Reserve memory
  *value = (double **) calloc(nIdx, sizeof(double *));
  *m = (int *) calloc(nIdx, sizeof(int));

  // Return all requested representations
  for (i=0; i<nIdx; i++) {
    // Export preprocessor
    if ( (pre = dynamic_cast<Preprocessor *>(object(idx[i]))) )
      (*value)[i] = pre->exportRepresentation((*m)+i);

    // Export algorithm
    else if ( (alg = dynamic_cast<Algorithm *>(object(idx[i]))) )
      (*value)[i] = alg->exportRepresentation((*m)+i);

    else {
      // Invalid object requested
      TheCsimError.add("Network::exportObject: can only export preprocessors and algorithms.\n");
      return -1;
    }
  }

  return 0;
}

/* ******************* END MICHAEL PFEIFFER ******************** */
