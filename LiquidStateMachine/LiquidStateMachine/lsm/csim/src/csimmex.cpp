
#include "csimmex.h"
#include "classlist.i"
#include "csimerror.h"
#include "mexnetwork.h"

/****************************************************************
** We need some global variables
****************************************************************/

//! Finally we need some pointer to the network we are using
MexNetwork *TheNetwork=0;

//! The largest integer number
int MAXINTEGER  = ~(1 << ((sizeof(int)*8)-1));

double simDT(void)
{
  return TheNetwork->dt;
}

double simTime(void)
{
  return TheNetwork->t;
}

Network *TheCurrentNetwork(void)
{
  return TheNetwork;
}

/****************************************************************
** Some helpers do parse the MatLab given input
****************************************************************/

#if !defined(max)
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif
#if !defined(min)
#define min(A, B) ((A) < (B) ? (A) : (B))
#endif

int getDouble(const mxArray *arg, double *x)
{
  if (!arg) return -1;
  int m = mxGetM(arg); 
  int n = mxGetN(arg); 
  if (!mxIsNumeric(arg) || mxIsComplex(arg) || 
      mxIsSparse(arg)  || !mxIsDouble(arg) || 
      (m != 1) || (n != 1)) {
    *x = 0.0;
    return -1;
  }
  *x = mxGetScalar(arg);
  return 0;
}

int getDoubleArray(const mxArray *arg, double **x, int *m, int *n)
{
  if (!arg) return -1;
  *m = mxGetM(arg);
  *n = mxGetN(arg);
  if (!mxIsNumeric(arg) || mxIsComplex(arg) ||  
      mxIsSparse(arg)  || !mxIsDouble(arg) ) {
    *x = NULL;
    return -1;
  }
  *x = mxGetPr(arg); 
  return 0;
}

int getDoubleVector(const mxArray *arg, double **x, int *n)
{
  if (!arg) return -1;  
  int m = min(mxGetM(arg),mxGetN(arg));
  *n = m*max(mxGetM(arg),mxGetN(arg));
  if (!mxIsNumeric(arg) ||  mxIsComplex(arg) ||  
      mxIsSparse(arg)  || !mxIsDouble(arg)  || m > 1 || m < 0) {
    *x = NULL;
    return -1;
  }
  *x = mxGetPr(arg); 
  return 0;
}

int getUint32Vector(const mxArray *arg, uint32 **x, int *n)
{
  if (!arg) return -1;
  int m = min(mxGetM(arg),mxGetN(arg));
  *n = max(mxGetM(arg),mxGetN(arg));
  if (!mxIsUint32(arg) || m != 1 ) {
    if (!mxIsNumeric(arg) ||  mxIsComplex(arg) ||  
        mxIsSparse(arg)  || !mxIsDouble(arg)  || m > 1 || m < 0) {
      *x = NULL;
      return -1;
    } else { // this is a double vector so we convert it to a uint32
      double *d = mxGetPr(arg);
      uint32 *u = (*x)  = (uint32 *)mxCalloc(*n,sizeof(uint32));
      for(int i=0;i<*n;i++) {
        u[i] = (uint32)(d[i]);
      }
      return 0;
    }
  }
  *x = (uint32 *)mxGetData(arg); 
  return 0;
}

int getUint32Scalar(const mxArray *arg, uint32 *x)
{
  if (!arg) return -1;
  int m = min(mxGetM(arg),mxGetN(arg));
  int n = max(mxGetM(arg),mxGetN(arg));
  if (!mxIsUint32(arg) || m != 1 || n!= 1) {
    *x = 0;
    return -1;
  }
  *x = (*((uint32 *)mxGetData(arg))); 
  return 0;
}

int getString(const mxArray *arg, char **str)
{
  if (!arg) return -1;
  if ( mxIsChar(arg) ) {
    int sl=(mxGetM(arg) * mxGetN(arg)) + 1;
    *str = (char *)mxCalloc(sl, sizeof(char));
    mxGetString(arg, *str, sl);
    return 0;
  } else {
    return -1;
  }
}



/**************************************************************************************
** Here we implement the actual commands
**************************************************************************************/

int csimMexInit(int , mxArray *[], int , const mxArray *[])
{
  if ( !TheNetwork ) {
    /*
    ** Create some fresh empty network
    */
    TheNetwork = new MexNetwork();
    return 0;
  }
  // else {
  //  mexPrintf("CSIM: Network already initialized. Command ignored.\n");
  //  return 0;
  // }
  return 0;
}

int csimMexCreate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
  /*
  ** Create some fresh empty network
  */
  if ( !TheNetwork ) TheNetwork = new MexNetwork();
  
  if ( nrhs < 2 || nrhs > 3 || nlhs != 1 )
    mexErrMsgTxt("CSIM-Usage: idx = csim('create',className[,n]);\n");

  char *className;
  if ( getString(prhs[1],&className) )
    mexErrMsgTxt("CSIM-Usage: idx = csim('create',className[,n]); className not a string.\n");

  int n=1; 
  if ( nrhs > 2 ) {
    double tmp;
    if ( getDouble(prhs[2],&tmp) )
      mexErrMsgTxt("CSIM-Usage: idx = csim('create',className[,n]); n not a single double.\n");
  
    n = (int)(tmp);
  }

  uint32 *idx = (uint32 *)mxCalloc(n,sizeof(uint32));
 
  int switchError = 0;
  //  #define __SWITCH_COMMAND__ { for(int i=0; i<n; i++) if ( (idx[i]=TheNetwork->addNewObject((Advancable *)(new TYPE))) < 0 ) { TheCsimError.add("can not add %s (i=%i)\n",className,i); return -1; } }
  Advancable *a; a=0;
#define __SWITCH_COMMAND__ { for(int i=0; i<n; i++) { \
                               idx[i]=TheNetwork->addNewObject(a=(Advancable *)(new TYPE)); \
                               if ( a->init(a) < 0 ) { \
                                 TheCsimError.add("csim('create',className,n): error calling init of %s!\n",className); \
                                 return -1; \
                               } \
                             } \
                           }
#include "switch.i"

  if ( switchError  < 0 ) {
    TheCsimError.add("csim('create',className,n): Unknown className %s!\n",className);
    return -1;
  } else {
    plhs[0] = mxCreateNumericMatrix ( 0, 0, mxUINT32_CLASS, mxREAL );
    mxSetM(plhs[0],1);
    mxSetN(plhs[0],n);
    mxSetData(plhs[0],(void *)idx);
  }
  
  return 0; 

}

int csimMexSet(int nlhs, mxArray *[], int nrhs, const mxArray *prhs[])
{

  if ( !TheNetwork ) 
    mexErrMsgTxt("CSIM: No network initialized yet!\n");

  if ( nrhs < 3 || nlhs > 0 )
    mexErrMsgTxt("CSIM-Usage: csim('set',idx,field,value[,field,value]*);\n");

  char *globalVar;
  if ( getString(prhs[1],&globalVar) == 0) {

    if ( nrhs != 3 )
      mexErrMsgTxt("CSIM-Usage: csim('set',field,value);\n");

    double tmp;
    if ( getDouble(prhs[2],&tmp) ) {
      mexErrMsgTxt("CSIM-Usage: csim('set',field,value); value not a single double\n");
    }
    
    if ( TheNetwork->setField(globalVar,tmp) < 0 ) {
      TheCsimError.add("csimMexSet: can not set field %s to %g!",globalVar,tmp); return -1;
    }

  } else {
    uint32 *idx; int nIdx;
    if ( getUint32Vector(prhs[1],&idx,&nIdx) )
      mexErrMsgTxt("CSIM-Usage: csim('set',idx[,field,value]*); idx is not a uint32  vector.\n");
    
    // loop over all <field,value> pairs
    int k=2;
    while ( k < nrhs ) {
      char *paramName;
      if ( getString(prhs[k],&paramName) )
        mexErrMsgTxt("CSIM-Usage: csim('set',idx,field,value[,field,value]*); field is not a string.\n");
      
      double *value; int m,n;
      if ( getDoubleArray(prhs[k+1],&value,&m,&n) )
        mexErrMsgTxt("CSIM-Usage: csim('set',idx,field,value[,field,value]*); value is not a double array.\n");
      
      if ( n < 1 )
        mexErrMsgTxt("CSIM-Usage: csim('set',idx,field,value[,field,value]*); value is empty!");
      
      if ( (n>1) && (n!=nIdx) )
        mexErrMsgTxt("CSIM-Usage: csim('set',idx,field,value[,field,value]*); size(value,2) != length(idx)\n");
      
      if ( TheNetwork->setParameter(idx,nIdx,paramName,value,m,n) < 0 ) {
        TheCsimError.add("csim('set',idx,field,value); failed\n");
        return -1;
      }
      k += 2;
    }

  }

  return 0;
}


/** Returns a struct description of a network object. */
mxArray *getMxClassInfo(Advancable *a)
{
  csimClassInfo *classInfo=a->getClassInfo();

  // figure out the number of RW fields
  int i,nRW=0;
  for(i=0;i<classInfo->nFields();i++)
    nRW += classInfo->isFieldRW(i);

  // write rw fields into cell array
  mxArray *fields = mxCreateCellMatrix(1,nRW);
  int ii=0;
  for(i=0;i<classInfo->nFields();i++)
    if ( classInfo->isFieldRW(i) )
      mxSetCell(fields,ii++,mxCreateString(classInfo->getFieldName(i)));

  // make struct array with fields/values
  const char **fieldNames = (const char **)malloc((classInfo->nFields()+3)*sizeof(char *));
  fieldNames[0] = "className";
  fieldNames[1] = "spiking";
  fieldNames[2] = "fields";
  for(i=0;i<classInfo->nFields();i++) {
    fieldNames[3+i] = classInfo->getFieldName(i);
  }
  mxArray *info = mxCreateStructMatrix(1,1,classInfo->nFields()+3,fieldNames);
  free(fieldNames);

  //
  // fill in values
  //

  // className
  mxSetField(info,0,"className",mxCreateString(classInfo->name));

  // spiking
  double spiking=0.0;
  SpikingSynapse *s=dynamic_cast<SpikingSynapse *>(a);
  if ( s ) spiking=1.0;
  SpikingNeuron *n=dynamic_cast<SpikingNeuron *>(a);
  if ( n ) spiking=1.0;
  mxSetField(info,0,"spiking",mxCreateScalarDouble(spiking));

  // fields
  mxSetField(info,0,"fields",fields);
  
  // fields and values
  for(ii=0, i=0;i<classInfo->nFields();i++) {
    int mm = a->getFieldSizeById((char *)a,i);
    mxArray *fv = mxCreateDoubleMatrix(mm,1,mxREAL);
    double *value=mxGetPr(fv);
    a->getFieldById((char *)a,i,value);
    mxSetField(info,0,classInfo->getFieldName(i),fv);
  }

  return info;  
}


/* ******************* BEGIN MICHAEL PFEIFFER ************************ */


/** Returns a struct description of the preprocessors of a readout. */
mxArray *getMxReadoutPreprocessors(Readout *r)
{

  // make struct array with fields/values
  const char *fieldNames[1] = { "Preprocessors" };

  mxArray *info = mxCreateStructMatrix(1,1,1, fieldNames);

  //
  // fill in values
  //

  // Get all preprocessor ids
  mxArray *pres;
  int nPre  = 0;
  if ( (nPre = r->getNumberPreprocessors()) > 0  ) {
    // Get preprocessor ids
    pres = mxCreateNumericMatrix ( 1, nPre,  mxUINT32_CLASS, mxREAL );
    r->getPreprocessorIDs((uint32 *)mxGetData(pres));
   
  } else {
    pres = mxCreateDoubleMatrix( 0, 0, mxREAL);
  }

  mxSetField(info,0,"Preprocessors", pres);

  return info;  
}

/** Returns a struct description of the filters of a readout. */
mxArray *getMxReadoutFilters(Readout *r)
{

  // make struct array with fields/values
  const char *fieldNames[2] = { "AnalogFilter", "SpikeFilter" };

  mxArray *info = mxCreateStructMatrix(1,1,2, fieldNames);

  //
  // fill in values
  //

  mxArray *analog_idx;
  mxArray *spike_idx;
  if (r->hasAnalogFilter()) {
    analog_idx = mxCreateNumericMatrix ( 1, 1, mxUINT32_CLASS, mxREAL );
    *(uint32 *)(mxGetData(analog_idx)) = r->getAnalogFilterID();
  }
  else {
    analog_idx = mxCreateDoubleMatrix(0, 0, mxREAL);
  }
  if (r->hasSpikeFilter()) {
    spike_idx = mxCreateNumericMatrix ( 1, 1, mxUINT32_CLASS, mxREAL );
    *(uint32 *)(mxGetData(spike_idx)) = r->getSpikeFilterID();
  }
  else {
    spike_idx = mxCreateDoubleMatrix(0, 0, mxREAL);
  }

  
  mxSetField(info,0,"AnalogFilter", analog_idx);
  mxSetField(info,0,"SpikeFilter", spike_idx);

  return info;  
}

/** Returns a struct description of the algorithm of a readout. */
mxArray *getMxReadoutAlgorithm(Readout *r)
{

  // make struct array with fields/values
  const char *fieldNames[1] = { "Algorithm" };

  mxArray *info = mxCreateStructMatrix(1,1,1, fieldNames);

  //
  // fill in values
  //

  mxArray *algo_idx;
  if (r->hasAlgorithm()) {
    algo_idx = mxCreateNumericMatrix ( 1, 1, mxUINT32_CLASS, mxREAL );
    *(uint32 *)(mxGetData(algo_idx)) = r->getAlgorithmID();
  }
  else {
    algo_idx = mxCreateDoubleMatrix(0, 0, mxREAL);
  }
  
  mxSetField(info,0,"Algorithm", algo_idx);

  return info;  
}


/** Returns a struct description of the inputs of a physical model. */
mxArray *getMxModelInputs(PhysicalModel *phm)
{
  // make struct array with fields/values
  const char *fieldNames[2] = { "InputNames", "InputConnections" };

  mxArray *info = mxCreateStructMatrix(1,1,2, fieldNames);

  //
  // fill in values
  //

  mxArray *names;
  mxArray *conns;
  int nIn  = 0;
  if ( (nIn = phm->nInChannels()) > 0  ) {
    // Get input-channel names

    names = mxCreateCellMatrix(1, nIn);
    conns = mxCreateCellMatrix(1, nIn);
    mxArray *actConns;
    uint32* conn_idx;

    for (int i=0; i<nIn; i++) {
      // Set connection name
      mxSetCell(names, i, mxCreateString(phm->getInputChannelName(i)));

      conn_idx = phm->getInputs(i);
      if (conn_idx) { // Set input index
	actConns = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
	*((uint32 *)mxGetData(actConns)) = *conn_idx;
	mxFree(conn_idx);
	mxSetCell(conns, i, actConns);
      }
      else // no connections yet
	mxSetCell(conns, i, mxCreateDoubleMatrix(0, 0, mxREAL));
    }

  } else {
    names = mxCreateDoubleMatrix( 0, 0, mxREAL);
    conns = mxCreateDoubleMatrix( 0, 0, mxREAL);
  }

  mxSetField(info,0,"InputNames", names);
  mxSetField(info,0,"InputConnections", conns);

  return info;  
}


/** Returns a struct description of the outputs of a physical model. */
mxArray *getMxModelOutputs(PhysicalModel *phm)
{
  // make struct array with fields/values
  const char *fieldNames[2] = { "OutputNames", "OutputConnections" };

  mxArray *info = mxCreateStructMatrix(1,1,2, fieldNames);

  //
  // fill in values
  //

  mxArray *names;
  mxArray *conns;

  int nOut  = 0;
  if ( (nOut = phm->nOutChannels()) > 0  ) {
    // Get output-channel names

    names = mxCreateCellMatrix(1, nOut);
    conns = mxCreateCellMatrix ( 1, nOut);
    mxArray *actConns;
    uint32* conn_idx;

    for (int i=0; i<nOut; i++) {
      // Set connection name
      mxSetCell(names, i, mxCreateString(phm->getOutputChannelName(i)));

      conn_idx = phm->getOutputs(i);
      if (conn_idx) { // Set output indices
	actConns = mxCreateNumericMatrix(1, phm->nOutputs(i), mxUINT32_CLASS, mxREAL);

        memcpy(mxGetPr(actConns),(void *) conn_idx,(phm->nOutputs(i))*sizeof(uint32));
	mxFree(conn_idx);
	mxSetCell(conns, i, actConns);
      }
      else // no connections yet
	mxSetCell(conns, i, mxCreateDoubleMatrix(0, 0, mxREAL));
    }

  } else {
    names = mxCreateDoubleMatrix( 0, 0, mxREAL);
    conns = mxCreateDoubleMatrix( 0, 0, mxREAL);
  }

  mxSetField(info,0,"OutputNames", names);
  mxSetField(info,0,"OutputConnections", conns);


  return info;  
}


/* ******************* END MICHAEL PFEIFFER ************************ */


/** Returns either all network parameters, the values of a global variable, values of an object, connections or parameter lists. */
int csimMexGet(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
  if ( !TheNetwork ) 
    mexErrMsgTxt("CSIM: No network initialized yet!\n");
  
  if ( nrhs < 1 || nrhs > 3 )
    mexErrMsgTxt("CSIM-Usage: value = csim('get',idx[,fieldName]); or\n"
                 "            value = csim('get'[,globalvar]); or\n"
                 "            [pre,post] = csim('get',idx,'connections');\n" );

  if ( nrhs < 2 ) {
    // csim('get'): return all network parameters
    TheNetwork->printFields(); return 0;
  }

  char *globalVar;
  if ( getString(prhs[1],&globalVar) == 0) {
    // csim('get', 'globalVar'): return value of a global variable
    if ( nrhs > 2 )
      mexErrMsgTxt("CSIM-Usage: csim('get',globalvar);");
    double tmp;
    if ( TheNetwork->getField(globalVar,&tmp) < 0 ) return -1;
    plhs[0] = mxCreateScalarDouble(tmp);
  } 


  else {
    // Return info about a network object
    Advancable *a;
    uint32 *idx; int nIdx;
    char *fieldName;

    // Get index of the object
    if ( getUint32Vector(prhs[1],&idx,&nIdx) )
      mexErrMsgTxt("CSIM-Usage: P=csim('get',idx[,fieldName]);  idx is not a uint32 vector.\n");
    
    if ( (a=TheNetwork->getObject(idx[0])) ){
      Recorder *r;
      if ( (r = dynamic_cast<Recorder *>(a)) && (nrhs > 2) ) {
	// Return recorder traces

        if ( getString(prhs[2],&fieldName) )
          mexErrMsgTxt("CSIM-Usage: P=csim('get',idx,fieldName); fieldName is not a string.\n");
        if ( (0 == strncmp(fieldName,"traces",strlen(fieldName))) ) {
          plhs[0] = r->getMxStructArray();
          return 0;
        }
      }

      /* *************** BEGIN MICHAEL PFEIFFER **************** */

      // Return readout info

      Readout *ro;
      if ( (ro = dynamic_cast<Readout *>(a)) && (nrhs > 2) ) {

	if ( getString(prhs[2],&fieldName) )
          mexErrMsgTxt("CSIM-Usage: P=csim('get',idx,fieldName); fieldName is not a string.\n");

	// Return readout filters
	if ( (0 == strncmp(fieldName,"filters",strlen(fieldName))) ) {
	  plhs[0] = getMxReadoutFilters(ro);
	  return 0;
	}

	// Return readout preprocessors
	if ( (0 == strncmp(fieldName,"preprocessors",strlen(fieldName))) ) {
	  plhs[0] = getMxReadoutPreprocessors(ro);
	  return 0;
	}

	// Return readout algorithm
	if ( (0 == strncmp(fieldName,"algorithm",strlen(fieldName))) ) {
	  plhs[0] = getMxReadoutAlgorithm(ro);
	  return 0;
	}

      }


      PhysicalModel *phm;
      if ( (phm = dynamic_cast<PhysicalModel *>(a)) && (nrhs > 2) ) {

	if ( getString(prhs[2],&fieldName) )
          mexErrMsgTxt("CSIM-Usage: P=csim('get',idx,fieldName); fieldName is not a string.\n");

	// Return model input names and connections
	if ( (0 == strncmp(fieldName,"inputs",strlen(fieldName))) ) {
	  plhs[0] = getMxModelInputs(phm);
	  return 0;
	}

	// Return model output names and connections
	if ( (0 == strncmp(fieldName,"outputs",strlen(fieldName))) ) {
	  plhs[0] = getMxModelOutputs(phm);
	  return 0;
	}
      }
      
      /* *************** END MICHAEL PFEIFFER **************** */
    
    } else {
      TheCsimError.add("csim('get',idx,...); idx(1) is not a valid object index!\n");
      return -1;
    }

    if ( nrhs == 3 ) {
      if ( getString(prhs[2],&fieldName) )
        mexErrMsgTxt("CSIM-Usage: P=csim('get',idx,fieldName); fieldName is not a string.\n");
      
      if ( 0 == strcmp(fieldName,"connections") ) {
	// Return connections of a network object

        if ( nlhs != 2 ) {
          mexErrMsgTxt("CSIM-Usage: [pre,post]=csim('get',idx,'connections');"
                       " needs two return arguments.\n");
        }
        if ( nIdx != 1 ) {
          mexErrMsgTxt("CSIM-Usage: [pre,post]=csim('get',idx,'connections');"
                       " idx must be a uint32 scalar.\n");
        }

        if ( (a=TheNetwork->getObject(idx[0])) ) {
	  // Return connections of neuron or synapse
	  
          Neuron  *n;
          Synapse *s;
          if ( (s = dynamic_cast<Synapse *>(a)) ) {
	    // Get synapse connections
            plhs[0] = mxCreateNumericMatrix ( 1, 1, mxUINT32_CLASS, mxREAL );
            plhs[1] = mxCreateNumericMatrix ( 1, 1, mxUINT32_CLASS, mxREAL );
            *(uint32 *)(mxGetData(plhs[0])) = s->getPre();
            *(uint32 *)(mxGetData(plhs[1])) = s->getPost();
          } else if ( (n = dynamic_cast<Neuron *>(a)) ) {
	    // Get neuron connections
            unsigned nPre  = 0;
            unsigned nPost = 0;
            if ( (nPre = n->nPre()) > 0  ) {
	      // Get presynaptic connections
              plhs[0] = mxCreateNumericMatrix ( 1, nPre,  mxUINT32_CLASS, mxREAL );
              n->getPre((uint32 *)mxGetData(plhs[0]));
            } else {
              plhs[0] = mxCreateDoubleMatrix ( 0, 0, mxREAL );
            }
            if ( (nPost = n->nPost()) > 0 ) {
	      // Get postsynaptic inputs
              plhs[1] = mxCreateNumericMatrix ( 1, nPost, mxUINT32_CLASS, mxREAL );
              n->getPost((uint32 *)mxGetData(plhs[1]));
            } else {
              plhs[1] = mxCreateDoubleMatrix ( 0, 0, mxREAL );
            }
          } else
            {TheCsimError.add("csim('get',idx,'connections'); idx(1) is not a synapse or neuron!\n"); return -1;}

        } else
          {TheCsimError.add("csim('get',idx,'connections'); idx(1) is not a valid object index!\n"); return -1;}

      } else if ( 0 == strcmp(fieldName,"struct") ) {
	// Return struct description of the object
        Advancable *a;
        if ( (a=TheNetwork->getObject(idx[0])) )
          plhs[0] = getMxClassInfo(a);
        else
          {TheCsimError.add("csim('get',idx,'struct'); idx(1) is not a valid object index!\n"); return -1;}
      } else {
	// Return single parameter value of several objects

        double *p=0; int m;
        if ( TheNetwork->getParameter(idx,nIdx,fieldName,&p,&m) < 0 ) {
          if ( p ) free(p); p=0;
          return -1;
        }
        
        plhs[0] = mxCreateDoubleMatrix(m, nIdx, mxREAL);
        memcpy(mxGetPr(plhs[0]),p,m*nIdx*sizeof(double));
        if ( p ) free(p); p=0;
      
        return 0;
      }
    } else if ( nrhs == 2 ) {
      // nrhs == 2: no fieldName given
      // so we print out the values of all registerd fields of idx[0]

      Advancable *a;
      if ( nlhs < 1 ) {
        for(int i=0; i<nIdx; i++) {
          if ( (a=TheNetwork->getObject(idx[i])) )
            a->printFields((char *)a);
          else
            {TheCsimError.add("csim('get',idx); idx(%i) is not a valid object index!\n",i+1); return -1;}
        }
      } else {
        if ( (a=TheNetwork->getObject(idx[0])) )
          plhs[0] = getMxClassInfo(a);
        else
          {TheCsimError.add("csim('get',idx); idx(1) is not a valid object index!\n"); return -1;}
      }
    }
  }

  return 0;
}


/** Connect parts of the network. */
int csimMexConnect(int , mxArray *[], int nrhs, const mxArray *prhs[])
{
  if ( !TheNetwork ) 
    mexErrMsgTxt("CSIM: No network initialized yet!\n");

  if ( nrhs < 3 || nrhs > 4 )
    mexErrMsgTxt("CSIM-Usage: csim('connect',dstIdx,srcIdx);" 
                 "            csim('connect',dstIdx,srcIdx,viaIdx); or\n"
                 "            csim('connect',recorderIdx,objectIdx,fieldname);\n");

  
  // Get destination indices
  uint32 *idx1; int nIdx1;
  if ( getUint32Vector(prhs[1],&idx1,&nIdx1) )
    mexErrMsgTxt("\nCSIM-Usage: csim('connect',dstIdx, ...); dstIdx is not a uint32 vector.\n");
  
  // Get target indices
  uint32 *idx2; int nIdx2;
  if ( getUint32Vector(prhs[2],&idx2,&nIdx2) )
    mexErrMsgTxt("\nCSIM-Usage: csim('connect',dstIdx, Idx, ...); Idx is not a uint32 vector.\n");
   
  if ( nIdx1 == 0 || nIdx2 == 0 ) 
    mexErrMsgTxt("\nCSIM-Usage: csim('connect',dstIdx, Idx, ...); empty index vectors!.\n");
  

  if ( nrhs == 4 ) {
    // Connect 'via' or to recorder or readout or physical model
    char *fieldName;
    if ( getString(prhs[3],&fieldName) < 0 ) {

      // Make connection 'via'
      uint32 *idx3; int nIdx3;
      if ( getUint32Vector(prhs[3],&idx3,&nIdx3) ) {
        mexErrMsgTxt("\nCSIM-Usage: csim('connect',dstIdx,srcIdx,viaIdx); srcIdx is not a uint32 vector.\n");
      } else {
        if ( nIdx1 == nIdx2 && nIdx1 == nIdx3 )
          TheNetwork->connect(idx1,idx2,idx3,nIdx1);
        else
          mexErrMsgTxt("\nCSIM-Usage: csim('connect',dstIdx,srcIdx,viaIdx); dstIdx, srcIdx, and viaIdx do not have the same length\n");
      }
    } else {

      // Make connection to recorder or readout or physical model
      if ( nIdx1 == 1 )
        TheNetwork->connect(idx1[0],idx2,nIdx2,fieldName);
      else
        mexErrMsgTxt("\nCSIM-Usage: csim('connect',recorderIdx, Idx, fieldname); more than one recorder specified!\n");
    } 
  } else {

    // Connect two objects or object lists
    if ( nIdx1 == nIdx2 )
      TheNetwork->connect(idx1,idx2,nIdx1);
    else if ( nIdx1 == 1 )
      TheNetwork->connect(idx1[0],idx2,nIdx2);
    else if ( nIdx2 == 1 )
      TheNetwork->connect(idx1,idx2[0],nIdx1);
    else
      mexErrMsgTxt("\nCSIM-Usage: csim('connect',dstIdx, srcIdx); dimension of dstIdx and srcIdx do not match!\n");
  }
  return 0;
}

int csimMexReset(int , mxArray *[], int nrhs, const mxArray *[])
{
  if ( !TheNetwork ) 
    mexErrMsgTxt("CSIM: No network initialized yet!\n");

  if ( nrhs != 1 )
    mexErrMsgTxt("CSIM-Usage: csim('reset');\n");

  TheNetwork->reset();

  return 0;
}

int csimMexList(int , mxArray *[], int nrhs, const mxArray *prhs[])
{
  if ( nrhs < 1 )
    mexErrMsgTxt("CSIM-Usage: csim('list'[,what[,options]]);\n");

  if ( nrhs == 1 ) {
    TheCsimClassDB.listClasses(0);
    return 0;
  }

  if ( nrhs > 1 ) {
    char *what;
    if ( getString(prhs[1],&what) < 0 )
      mexErrMsgTxt("CSIM-Usage: csim('list',<what>[,options]); <what> is not a string!\n");
    if ( 0 == strncmp(what,"classes",strlen(what)) ) {
      bool F=0;
      if (nrhs > 2) {
        char *opt;
        if ( getString(prhs[2],&opt) < 0 )
          mexErrMsgTxt("CSIM-Usage: csim('list','classes',option); option is not a string!\n");
        if ( 0 == strncmp(opt,"-fields",strlen(opt)) )
          F = 1;
        else
          mexErrMsgTxt("CSIM-Usage: csim('list','classes',option);  option is unknown!\n");
      }
      TheCsimClassDB.listClasses(F);
    } else  if ( 0 == strncmp(what,"objects",strlen(what)) ) {
      bool F=0;
      if (nrhs > 2) {
        char *opt;
        if ( getString(prhs[2],&opt) < 0 )
          mexErrMsgTxt("CSIM-Usage: csim('list','objects',option); option is not a string!\n");
        if ( 0 == strncmp(opt,"-fields",strlen(opt)) )
          F = 1;
        else
          mexErrMsgTxt("CSIM-Usage: csim('list','objects',option);  option is unknown!\n");
      }
      TheNetwork->listObjects(F);
    } else {
      mexErrMsgTxt("CSIM-Usage: csim('list',<what>[,options]); <what> is unknown!\n");
    }
  }

  return 0;
}

int csimMexSimulate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
  int i;
  int n, m, nInputs, nSigArg, nTeachers = 0;
  int i_spiking, i_dt, i_data, i_idx;

  if ( !TheNetwork )
    mexErrMsgTxt("CSIM: No network initialized yet!\n");

  if ( nrhs < 2 || nlhs > 1)
    mexErrMsgTxt("CSIM-Usage: [output = ]csim('simulate',Tsim,[,InputSignal]*);\n");

  double Tsim;
  if ( getDouble(prhs[1],&Tsim) )
    mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Tsim is not a single double.\n");

  // prhs[2,...] should be something like a struct or an empty matrix

  nInputs=0;
  csimInputChannel *inputs=0;
  csimInputChannel *teachers=0;

  int k=2;
  while ( k < nrhs ) {
    n = mxGetN(prhs[k]);
    m = mxGetM(prhs[k]);
    nSigArg = max(n,m);

    if ( nSigArg > 0 && !mxIsStruct(prhs[k]) )
      mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input is non empty but not a struct array.\n");

    if ( nSigArg == 0 )
      mexPrintf("CSIM-Warning: input is empty!\n");

    if ( nSigArg > 0 && min(m,n) != 1 )
      mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input is not a struct vector.\n");
    

    double tmp=0.0;
    if ( nSigArg > 0 ) {

      i_spiking = mxGetFieldNumber(prhs[k],"spiking");
      if ( i_spiking < 0 )
        mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input has no field 'spiking' \n");
      
      i_dt   = mxGetFieldNumber(prhs[k],"dt");
      if ( i_dt < 0 )
        mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input has no field 'dt' \n");
      
      i_data = mxGetFieldNumber(prhs[k],"data");
      if ( i_data < 0 )
        mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input has no field 'data' \n");
      
      i_idx = mxGetFieldNumber(prhs[k],"idx");
      if ( i_idx < 0 )
        mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input has no field 'idx' \n");
      
      inputs = (csimInputChannel *)mxRealloc(inputs,sizeof(csimInputChannel)*(nInputs+nSigArg));

      //      printf("after realloc %i\n",nSigArg);

      for(i=0; i<nSigArg; i++) {
        if ( getDouble(mxGetFieldByNumber(prhs[k], i, i_spiking),&tmp) < 0 )
          mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input(i).spiking is not scalar \n");
        
        inputs[nInputs+i].dt      = ( tmp > 0.0 ) ? -1 : 0;
        
        if ( getDouble(mxGetFieldByNumber(prhs[k], i, i_dt),&tmp) < 0 )
          mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input(i).dt is not scalar \n");
        
        if ( inputs[nInputs+i].dt == 0 )
          inputs[nInputs+i].dt = tmp;
        
        if ( getDoubleVector(mxGetFieldByNumber(prhs[k], i, i_data),&(inputs[nInputs+i].data),&(inputs[nInputs+i].length)) )
          mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input(i).data is not a double vector \n");
        
        if ( getUint32Vector(mxGetFieldByNumber(prhs[k], i, i_idx),&(inputs[nInputs+i].idx),&(inputs[nInputs+i].nIdx)) )
          mexErrMsgTxt("CSIM-Usage: csim('simulate',Tsim[,InputSignal]*); Input(i).odx is not a uint32 scalar \n");
      }

      nInputs += nSigArg;

    }

    k++;

  }

  if ( TheNetwork->simulate((unsigned long)(Tsim/DT+0.5),inputs,nInputs,teachers,nTeachers) < 0)
    { TheCsimError.add("CSIM: csim('simulate',Tsim[,InputSignal]*); failed!\n"); return -1; }

  //now we have to output the recorded stuff!
  if (nlhs > 0)
    plhs[0] = TheNetwork->getMexOutput();

  return 0;
}

/* ********************** BEGIN MICHAEL PFEIFFER ******************* */
// Export the whole network or a parametric description of preprocessors or algorithms
int csimMexExport(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if ( !TheNetwork ) 
    mexErrMsgTxt("CSIM: No network initialized yet!\n");

  if ( nrhs == 1 ) {
    // Export the whole network
    if ( nlhs > 0 )
      plhs[0] = TheNetwork->exportNetwork();
  }

  else if ( nrhs == 2 ) {
    // Export a preprocessor or an algorithm
    if ( nlhs > 0) {

      // Get identifiers of preprocessors or algorithms
      uint32 *idx; int nIdx;
      if ( getUint32Vector(prhs[1],&idx,&nIdx) )
	mexErrMsgTxt("CSIM-Usage: csim('set',idx[,field,value]*); idx is not a uint32  vector.\n");
    

        double **p=0; int *m;
        if ( TheNetwork->exportObject(idx,nIdx,&p,&m) < 0 ) {
	  // Something went wrong clear all reserved memory
          if ( p ) {
	    for (int i=0; i<nIdx; i++) {
	      if (p[i]) {
		free(p[i]); p[i]=0;
	      }
	    }
	    free(p); p=0;
	  }
	  if (m) {
	    free(m); m=0;
	  }
          return -1;
        }
	
        plhs[0] = mxCreateCellMatrix(1, nIdx);

	// Copy the export values to a cell array
	mxArray *representation;
	for (int i=0; i<nIdx; i++) {
	  representation = mxCreateDoubleMatrix(1, m[i], mxREAL);
	  memcpy(mxGetPr(representation),p[i],m[i]*sizeof(double));
	  mxSetCell(plhs[0], i, representation);
	}

	// Clear the memory
	if ( p ) {
	  for (int i=0; i<nIdx; i++) {
	    if (p[i]) {
	      free(p[i]); p[i]=0;
	    }
	  }
	  free(p); p=0;
	}
	if (m) {
	    free(m); m=0;
	}
      
        return 0;
      
    }
    else
      mexErrMsgTxt("CSIM-Usage: values = csim('export', idx);\n");
  }
  else
    mexErrMsgTxt("CSIM-Usage: net = csim('export'); or\n"
                 "            values = csim('export', idx);\n" );

  return 0;
}
/* ********************** END MICHAEL PFEIFFER ******************* */


/* ******************* BEGIN MICHAEL PFEIFFER ********************* */
// Import a whole network or a parametric description of preprocessors or algorithms
int csimMexImport(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
  if (nrhs == 3) {
    // Import preprocessor or filter

    // Get identifiers of preprocessors or algorithms
    uint32 *idx; int nIdx;
    if ( getUint32Vector(prhs[1],&idx,&nIdx) )
      mexErrMsgTxt("CSIM-Usage: csim('set',idx[,field,value]*); idx is not a uint32  vector.\n");
    

    // Get description data
    double *values; int n;
    if ( getDoubleVector(prhs[2],&values,&n) )
      mexErrMsgTxt("CSIM-Usage: csim('import',idx,values; values is not a double vector.\n");

    if ( n < 1 )
      mexErrMsgTxt("CSIM-Usage: csim('import',idx,values; values is empty!");
      
    // Import the data
    if ( TheNetwork->importObject(idx,nIdx,values,n) < 0 ) {
      TheCsimError.add("csim('import',idx,values; failed\n");
      return -1;
    }

    return 0;

  }
  else if (nrhs == 2) {
    // Import a whole network

    if ( !TheNetwork ) 
      TheNetwork = new MexNetwork();

    return TheNetwork->importNetwork(prhs[1]);
  }
  else
    mexErrMsgTxt("CSIM-Usage: csim('import',net);\n");

}
/* ******************* END MICHAEL PFEIFFER ********************* */

int csimMexDestroy(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
  // 
  // Delete the network
  //
  if (TheNetwork) {
    int n=TheNetwork->getNumObjects();
    delete TheNetwork;

    if ( n > 0 )
      mexPrintf("CSIM: network deleted.\n");

    TheNetwork=0;
  }

  csimMexInit(nlhs,plhs,nrhs,prhs);

  return 0;
}




//! When the CSIM MEX-file is cleared (e.g. at an exit from matlab) this function will be called.
/** This function is registered as MEX exit function via
 ** mexAtExit(). Within this function one has to make sure that we
 ** "clean up" properly. */
void csimMexCleanUp(void) {
  // mexPrintf("CSIM: MEX-file is cleared, destroying/freeing all internal data.\n");
  // 
  // Delete the network
  //
  if (TheNetwork) {

    int n=TheNetwork->getNumObjects();
    delete TheNetwork;

    if ( n > 0 )
      mexPrintf("CSIM: network deleted.\n");

    TheNetwork=0;

  }
}
