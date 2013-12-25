/*! \file  mexnetwork.cpp
**  \brief Implementation of Network
*/

#include "mexnetwork.h"
#include "mexrecorder.h"
#include "csimlist.h"
#include "csimerror.h"
#include "csimmex.h"
/* *********** BEGIN MICHAEL PFEIFFER ********* */
#include "readout.h"
#include "physicalmodel.h"
/* *********** END MICHAEL PFEIFFER ********* */

#define object(i) (objectList.elements[(i)])    
#define nObjects  (objectList.n)    

/* ******** BEGIN MICHAEL PFEIFFER ************ */
#define preprocessors(i) (preprocessorList.elements[(i)])
#define nPreprocessors  (preprocessorList.n)

#define algorithms(i) (algorithmList.elements[(i)])
#define nAlgorithms  (algorithmList.n)
/* ********** END MICHAEL PFEIFFER ************ */

MexNetwork::MexNetwork() {
}


MexNetwork::~MexNetwork() {
}

int MexNetwork::addNewObject(Advancable *a)
{
  
  MexRecorder *r=dynamic_cast<MexRecorder *>(a);
  
  if ( r ) mexRecorderList.add(r);
  
  return Network::addNewObject(a);

}

mxArray *MexNetwork::getMexOutput(void)
{

  mxArray *out_a = mxCreateCellMatrix(1,mexRecorderList.n+(spikeOutput>0));

  for(unsigned i=0;i<mexRecorderList.n;i++)
    mxSetCell(out_a,i,mexRecorderList.elements[i]->getMxStructArray());

  // as the last cell we always output the spikes in the form
  // ST.idx [uint32 array], ST.times [double array]
  /*
  ** output the spike times of all but the input neurons
  */

  if ( spikeOutput ) {
    unsigned long nSpikes = totalSpikeCount();
    
    const char *st_fields[]= { "idx", "times" };
    mxArray *st_a = mxCreateStructMatrix(1,1,2,st_fields);


    if ( nSpikes > 0 ) {
      mxArray *idx_a   = mxCreateNumericMatrix ( 1, nSpikes, mxUINT32_CLASS, mxREAL );
      mxArray *times_a = mxCreateDoubleMatrix( 1, nSpikes, mxREAL);
      
      mxSetFieldByNumber(st_a,0,0,idx_a);
      mxSetFieldByNumber(st_a,0,1,times_a);
      
      uint32 *idx  = (uint32 *)mxGetData(idx_a);
      double *times= mxGetPr(times_a);
      
      getSpikes(idx,times);
    }
    
    mxSetCell(out_a,mexRecorderList.n,st_a);
  }

  return out_a;
}

/* *************************** BEGIN MICHAEL PFEIFFER *************************** */
mxArray *MexNetwork::exportNetwork(void)
{
  int sz,nCon=0;

  const char *fields[]= { "globals", "object", "dst", "src", "recorderInfo", "readoutInfo", "preprocessorInfo", "algorithmInfo", "modelInfo", "version" };
  
  mxArray *mxNet = mxCreateStructMatrix(1,1,sizeof(fields)/sizeof(char *),fields);
  mxSetField(mxNet,0,"version",mxCreateString("$Id: mexnetwork.cpp,v 1.10 2004/07/09 10:15:48 pfeiffer Exp $"));

  mxArray *src_a;  
  if ( nSrc > 0 ) { 
    src_a = mxCreateNumericMatrix ( 1, nSrc, mxUINT32_CLASS, mxREAL );
    memcpy((uint32 *)mxGetData(src_a),src,nSrc*sizeof(uint32));
  } else {
    src_a = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"src",src_a);
  
  mxArray *dst_a;
  if ( nDst > 0 ) { 
    dst_a = mxCreateNumericMatrix ( 1, nDst, mxUINT32_CLASS, mxREAL );
    memcpy((uint32 *)mxGetData(dst_a),dst,nDst*sizeof(uint32));
  } else {
    dst_a = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"dst",dst_a);

  nCon += nDst;
  
  mxArray *rec_a;
  if ( nRecMem > 0 ) { 
    rec_a = mxCreateNumericMatrix ( 1, nRecMem, mxUINT8_CLASS, mxREAL );
    memcpy(mxGetData(rec_a),RecMem,nRecMem);
  } else {
    rec_a = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"recorderInfo",rec_a);


  // Export info about readout connections
  mxArray *mxReadouts;
  if ( nReadoutMem > 0 ) { 
    mxReadouts = mxCreateNumericMatrix ( 1, nReadoutMem, mxUINT8_CLASS, mxREAL );
    memcpy(mxGetData(mxReadouts),ReadoutMem,nReadoutMem);
  } else {
    mxReadouts = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"readoutInfo",mxReadouts);
  


  // Export the info about preprocessors
  mxArray *mxPreprocessors;
  if ( nPreprocessors > 0 ) {
    const char *prepfields[]= { "index", "parameter" };
    mxPreprocessors = mxCreateStructMatrix(1,nPreprocessors,sizeof(prepfields)/sizeof(char *),prepfields);
    int index_i  = mxGetFieldNumber(mxPreprocessors,"index");
    int param_i = mxGetFieldNumber(mxPreprocessors,"parameter");

    mxArray *mxIndex;
    mxArray *mxParam;
    int rep_length;

    for(unsigned i=0;i<nPreprocessors;i++) {
      mxIndex = mxCreateNumericMatrix(1,1, mxUINT32_CLASS, mxREAL);
      uint32 prep_id = preprocessors(i)->getId();
      memcpy((uint32 *)mxGetData(mxIndex), &prep_id, sizeof(uint32));
      mxSetFieldByNumber(mxPreprocessors,i,index_i, mxIndex);

      double *p = preprocessors(i)->exportRepresentation(&rep_length);
      mxParam = mxCreateDoubleMatrix( 1, rep_length, mxREAL);
      memcpy(mxGetPr(mxParam), p, rep_length*sizeof(double));
      if (p) free(p); p=0;
      mxSetFieldByNumber(mxPreprocessors, i, param_i, mxParam);
    }
  } else {
    mxPreprocessors = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"preprocessorInfo",mxPreprocessors);


  // Export the info about algorithms
  mxArray *mxAlgorithms;
  if ( nAlgorithms > 0 ) {
    const char *algfields[]= { "index", "parameter" };
    mxAlgorithms = mxCreateStructMatrix(1,nAlgorithms,sizeof(algfields)/sizeof(char *),algfields);
    int index_i  = mxGetFieldNumber(mxAlgorithms,"index");
    int param_i = mxGetFieldNumber(mxAlgorithms,"parameter");

    mxArray *mxIndex;
    mxArray *mxParam;
    int rep_length;

    for(unsigned i=0;i<nAlgorithms;i++) {
      mxIndex = mxCreateNumericMatrix(1,1, mxUINT32_CLASS, mxREAL);
      uint32 alg_id = algorithms(i)->getId();
      memcpy((uint32 *)mxGetData(mxIndex), &alg_id, sizeof(uint32));
      mxSetFieldByNumber(mxAlgorithms,i,index_i, mxIndex);

      double *p = algorithms(i)->exportRepresentation(&rep_length);
      mxParam = mxCreateDoubleMatrix( 1, rep_length, mxREAL);
      memcpy(mxGetPr(mxParam), p, rep_length*sizeof(double));
      if (p) free(p); p=0;
      mxSetFieldByNumber(mxAlgorithms, i, param_i, mxParam);
    }
  } else {
    mxAlgorithms = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"algorithmInfo",mxAlgorithms);
  

  // Export info about physical-model connections
  mxArray *mxModels;
  if ( nModelMem > 0 ) { 
    mxModels = mxCreateNumericMatrix ( 1, nModelMem, mxUINT8_CLASS, mxREAL );
    memcpy(mxGetData(mxModels),ModelMem,nModelMem);
  } else {
    mxModels = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  mxSetField(mxNet,0,"modelInfo",mxModels);
  
  
  // export the fields of the Network
  mxArray *g_a;
  g_a = mxCreateDoubleMatrix ( 1, sz=this->getFieldArraySize(), mxREAL );
  double *p=this->getFieldArray();
  memcpy(mxGetPr(g_a),p,sz*sizeof(double));
  if (p) free(p); p=0;
  mxSetField(mxNet,0,"globals",g_a);
  
  mxArray *mxObjects;
  if ( nObjects > 0 ) {
    const char *objfields[]= { "type", "parameter" };
    mxObjects = mxCreateStructMatrix(1,nObjects,sizeof(objfields)/sizeof(char *),objfields);
    int type_i  = mxGetFieldNumber(mxObjects,"type");
    int param_i = mxGetFieldNumber(mxObjects,"parameter");
    mxArray *mxParam;
    for(unsigned i=0;i<nObjects;i++) {
      mxSetFieldByNumber(mxObjects,i,type_i,mxCreateString(object(i)->className()));
      sz=object(i)->getFieldArraySize();
      mxParam = mxCreateDoubleMatrix ( 1, sz, mxREAL );
      double *p=object(i)->getFieldArray((char *)object(i));
      memcpy(mxGetPr(mxParam),p,sz*sizeof(double));
      if (p) free(p); p=0;
      mxSetFieldByNumber(mxObjects,i,param_i,mxParam);
    }
  } else {
    mxObjects = mxCreateDoubleMatrix ( 0, 0, mxREAL );
  }
  csimPrintf("CSIM: %i objects and %i connections exported\n",nObjects,nCon);

  mxSetField(mxNet,0,"object",mxObjects);

  return mxNet;

}
/* *************************** END MICHAEL PFEIFFER *************************** */


#define max(A, B) ((A) > (B) ? (A) : (B))
#define MAXTYPENAMELENGTH 2000

#include "classlist.i"

/* *************************** BEGIN MICHAEL PFEIFFER *************************** */
int MexNetwork::importNetwork(const mxArray *mxNet)
{
  if ( nObjects > 0 ) {
    TheCsimError.add("MexNetwork::importMexNetwork: can not merge networks!\n");
    return -1;
  }

  mxArray *mxObjects=mxGetField(mxNet,0,"object");
  if ( !mxObjects ) {
    TheCsimError.add("MexNetwork::importNetwork: input is no struct array with field 'object'!\n");
    return -1;
  }

  mxArray *mxDst=mxGetField(mxNet,0,"dst");
  if ( !mxDst ) {
    TheCsimError.add("MexNetwork::importNetwork: input is no struct array with field 'dst'!\n");
    return -1;
  }

  mxArray *mxSrc=mxGetField(mxNet,0,"src");
  if ( !mxSrc ) {
    TheCsimError.add("MexNetwork::importNetwork: input is no struct array with field 'src'!\n");
  }

  mxArray *mxRec=mxGetField(mxNet,0,"recorderInfo");
  if ( !mxRec ) {
    TheCsimError.add("MexNetwork::importNetwork: input is no struct array with field 'recorderInfo'!\n");
  }

  mxArray *mxGlob=mxGetField(mxNet,0,"globals");
  if ( !mxGlob ) {
    TheCsimError.add("MexNetwork::importNetwork: input is no struct array with field 'globals'!\n");
  }

  mxArray *mxReadout=mxGetField(mxNet,0,"readoutInfo");
  if ( !mxReadout ) {
    // Only issue a warning (backwards compatibility)
    csimPrintf("MexNetwork::importNetwork: WARNING: input is no struct array with field 'readoutInfo'!\n");
  }

  mxArray *mxPreprocessors=mxGetField(mxNet,0,"preprocessorInfo");
  if ( !mxPreprocessors ) {
    // Only issue a warning (backwards compatibility)
    csimPrintf("MexNetwork::importNetwork: WARNING: input is no struct array with field 'preprocessorInfo'!\n");
  }

  mxArray *mxAlgorithms=mxGetField(mxNet,0,"algorithmInfo");
  if ( !mxAlgorithms ) {
    // Only issue a warning (backwards compatibility)
    csimPrintf("MexNetwork::importNetwork: WARNING: input is no struct array with field 'algorithmInfo'!\n");
  }

  mxArray *mxModel=mxGetField(mxNet,0,"modelInfo");
  if ( !mxModel ) {
    // Only issue a warning (backwards compatibility)
    csimPrintf("MexNetwork::importNetwork: WARNING: input is no struct array with field 'modelInfo'!\n");
  }

  // Import global variables
  int nm;
  if ( (nm=mxGetN(mxGlob)*mxGetM(mxGlob)) == this->getFieldArraySize() ) {
    this->setFieldArray(mxGetPr(mxGlob));
  } else {
    TheCsimError.add("MexNetwork::importNetwork: length of 'globals' (%i) != %i which is required by Network!\n\n"
		     ,nm,this->getFieldArraySize());
    return -1;    
  }
  

  // Import objects
  int nObj    = max(mxGetN(mxObjects),mxGetM(mxObjects));
  int type_i  = mxGetFieldNumber(mxObjects,"type");
  int param_i = mxGetFieldNumber(mxObjects,"parameter");
  if ( type_i < 0 || param_i < 0 ) {
    TheCsimError.add("MexNetwork::importNetwork: field 'object' is not a struct array with fields 'type' and 'parameter'!\n");
    return -1;    
  }

  // Create all objects
  for(int i=0;i<nObj;i++) {
    mxArray *mxCN = mxGetFieldByNumber(mxObjects,i,type_i);
    if ( !mxCN ) {
      TheCsimError.add("MexNetwork::importNetwork:  mxGetFieldByNumber(mxObjects,i,type_i) failed \n");
      return -1;    
    }
    char *className;
    if ( getString(mxCN,&className) ) {
      TheCsimError.add("MexNetwork::importNetwork: mxGetString(mxCN,className,MAXTYPENAMELENGTH) failed \n");
      return -1;
    }

    int switchError = 0;
    Advancable *a;

    #define __SWITCH_COMMAND__  { (addNewObject(a=(Advancable *)(new TYPE))); \
                                   if ( a->init(a) < 0 ) { \
                                     TheCsimError.add("MexNetwork::importNetwork: error calling init of %s!\n",className); \
                                     return -1; \
                                   } \
				}

    #include "switch.i"

    if ( switchError ) {
      TheCsimError.add("Error importing network: could not create object!\n");
      return -1;
    }

    mxArray *mxParam = mxGetFieldByNumber(mxObjects,i,param_i);
    if ( (nm=mxGetN(mxParam)*mxGetM(mxParam)) == object(i)->getFieldArraySize() ) {
      object(i)->setFieldArray((char *)(object(i)),mxGetPr(mxParam));
    } else {
      TheCsimError.add("MexNetwork::importNetwork: length of object(%i).parameter (%i) != %i which is required by %s!\n\n"
		       ,i,nm,object(i)->getFieldArraySize(),object(i)->className());
      return -1;    
    }
  }
  csimPrintf("CSIM: %i objects",nObjects);


  // Create connections
  unsigned nCon = max(mxGetN(mxDst),mxGetM(mxDst));

  if ( nCon > 0 ) {
    uint32 *dst = (uint32 *)mxGetData(mxDst);
    uint32 *src = (uint32 *)mxGetData(mxSrc);
    for(unsigned j=0;j<nCon;j++)
      connect(dst[j],src[j]);
  }
  csimPrintf(" and %i connections imported\n",nCon);

  // Import recorder connections
  char *r       = (char *)mxGetData(mxRec);     // pointer to current position in the recorder call data
  int  nMem = max(mxGetN(mxRec),mxGetM(mxRec)); // number of bytes of recorder call data

  uint32 rIdx,n;
  while ( r - (char *)mxGetData(mxRec) < nMem )  {
    memcpy(&rIdx,r,sizeof(uint32));    r += sizeof(uint32);
    memcpy(&n,r,sizeof(uint32));       r += sizeof(uint32);
    uint32 *objIdx=(uint32 *)r;        r += sizeof(uint32)*n;
    char *fieldName=(char *)r;         r += strlen(fieldName)+1;
 
    connect(rIdx,objIdx,n,fieldName);
  }

  // Import readout connections
  if (mxReadout) {
    char *ro       = (char *)mxGetData(mxReadout);     // pointer to current position in the readout call data
    int  nReadoutMem = max(mxGetN(mxReadout),mxGetM(mxReadout)); // number of bytes of readout call data
    
    while ( ro - (char *)mxGetData(mxReadout) < nReadoutMem )  {
      memcpy(&rIdx,ro,sizeof(uint32));    ro += sizeof(uint32);
      memcpy(&n,ro,sizeof(uint32));       ro += sizeof(uint32);
      uint32 *objIdx=(uint32 *)ro;        ro += sizeof(uint32)*n;
      char *fieldName=(char *)ro;         ro += strlen(fieldName)+1;
      
      connect(rIdx,objIdx,n,fieldName);
    }
  }

  // Import preprocessor representations
  if (mxPreprocessors) {
    int nPrep    = max(mxGetN(mxPreprocessors),mxGetM(mxPreprocessors));
    if (nPrep > 0) {
      int index_i  = mxGetFieldNumber(mxPreprocessors,"index");
      int param_i = mxGetFieldNumber(mxPreprocessors,"parameter");
      if ( index_i < 0 || param_i < 0 ) {
	TheCsimError.add("MexNetwork::importNetwork: field 'preprocessorInfo' is not a struct array with fields 'index' and 'parameter'!\n");
	return -1;    
      }
      
      // Import each single preprocessor
      Preprocessor *pre = 0;
      for(int i=0;i<nPrep;i++) {
	mxArray *mxID = mxGetFieldByNumber(mxPreprocessors,i,index_i);
	if ( !mxID ) {
	  TheCsimError.add("MexNetwork::importNetwork:  mxGetFieldByNumber(mxPreprocessors,i,index_i) failed \n");
	  return -1;    
	}
	
	uint32 *idx; int nIdx;
	if ( getUint32Vector(mxID, &idx, &nIdx) ) {
	  TheCsimError.add("MexNetwork::importNetwork: mxGetUnit32Vector(mxID,&idx, &nidx) failed \n");
	  return -1;
	}
	
	mxArray *mxParam = mxGetFieldByNumber(mxPreprocessors,i,param_i);
	double *p = mxGetPr(mxParam);
	int rep_length = mxGetN(mxParam) * mxGetM(mxParam);
	
	
	if (pre = dynamic_cast<Preprocessor *>(object(*idx))) {
	  if ( pre->importRepresentation(p, rep_length) < 0 ) {
	    TheCsimError.add("MexNetwork::importNetwork: import preprocessor (%i) failed!\n\n", *idx);
	    return -1;    
	  }
	}
	else {
	  TheCsimError.add("MexNetwork::importNetwork: index %i is not a preprocessor!\n\n", *idx);
	  return -1;    
	}
      }
    }
  }

  // Import algorithm representations
  Algorithm *alg = 0;
  if (mxAlgorithms) {
    int nAlgo    = max(mxGetN(mxAlgorithms),mxGetM(mxAlgorithms));
    if (nAlgo > 0) {
      int index_i  = mxGetFieldNumber(mxAlgorithms,"index");
      int param_i = mxGetFieldNumber(mxAlgorithms,"parameter");
      if ( index_i < 0 || param_i < 0 ) {
	TheCsimError.add("MexNetwork::importNetwork: field 'algorithmInfo' is not a struct array with fields 'index' and 'parameter'!\n");
	return -1;    
      }
      
      // Import each single algorithm
      for(int i=0;i<nAlgo;i++) {
	mxArray *mxID = mxGetFieldByNumber(mxAlgorithms,i,index_i);
	if ( !mxID ) {
	  TheCsimError.add("MexNetwork::importNetwork:  mxGetFieldByNumber(mxAlgorithms,i,index_i) failed \n");
	  return -1;    
	}
	
	uint32 *idx; int nIdx;
	if ( getUint32Vector(mxID, &idx, &nIdx) ) {
	  TheCsimError.add("MexNetwork::importNetwork: mxGetUnit32Vector(mxID,&idx, &nidx) failed \n");
	  return -1;
	}
	
	mxArray *mxParam = mxGetFieldByNumber(mxAlgorithms,i,param_i);
	double *p = mxGetPr(mxParam);
	int rep_length = mxGetN(mxParam) * mxGetM(mxParam);
	
	if (alg = dynamic_cast<Algorithm *>(object(*idx))) {
	  if ( alg->importRepresentation(p, rep_length) < 0 ) {
	    TheCsimError.add("MexNetwork::importNetwork: import algorithm (%i) failed!\n\n", *idx);
	    return -1;    
	  }
	}
	else {
	  TheCsimError.add("MexNetwork::importNetwork: index %i is not an algorithm!\n\n", *idx);
	  return -1;    
	}
      }
    }
  }


  // Import physical-model connections
  if (mxModel) {
    char *phm       = (char *)mxGetData(mxModel);     // pointer to current position in the model call data
    int  nModelMem = max(mxGetN(mxModel),mxGetM(mxModel)); // number of bytes of model call data
    
    while ( phm - (char *)mxGetData(mxModel) < nModelMem )  {
      memcpy(&rIdx,phm,sizeof(uint32));    phm += sizeof(uint32);
      memcpy(&n,phm,sizeof(uint32));       phm += sizeof(uint32);
      uint32 *objIdx=(uint32 *)phm;        phm += sizeof(uint32)*n;
      char *fieldName=(char *)phm;         phm += strlen(fieldName)+1;
      
      connect(rIdx,objIdx,n,fieldName);
    }
  }



 
  return 0;
}

/* *************************** END MICHAEL PFEIFFER *************************** */


