/************************************************************************************
 **
 **  Implement the MEX interface for
 **
 **    [MI,HX,HY,HXY,bayes,tables] = mibayes(X,Y,evalSize,handle)
 **
 **  where evalSize, handle, HX, HY, HXY, bayes and tables are optional.
 **
 **  Compile with: mex mibayes.c mi_and_bayes.c
 **
 **  The real stuff is implemented in mi_and_bayes.c and mi_and_bayes.h
 **
 ************************************************************************************/

#include <math.h>
#include <mex.h>
#include "mi_and_bayes.h"


#if !defined(max)
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif
#if !defined(min)
#define min(A, B) ((A) < (B) ? (A) : (B))
#endif

/* usage/error */
#define errorString "MIBAYES-Error: [MI,HX,HY,HXY,bayes,tables] = mibayes(X,Y,evalSize,distHandle)\n                   "
#define usageString "MIBAYES-Usage: [MI,HX,HY,HXY,bayes,tables] = mibayes(X,Y,evalSize,distHandle); or\n                   tables = mibayes(distHandle);\n"

/* input arguments */
#define argVal(i)    (prhs[(i)])
#define argGiven(i)  (nrhs > (i))

#define arg_X          0
#define arg_Y          1
#define arg_evalSize   2
#define arg_distHandle 3

/* return values */
#define retVal(i)   (plhs[(i)])
#define retGiven(i) (nlhs > (i))

#define ret_MI        0
#define ret_HX        1
#define ret_HY        2
#define ret_HXY       3
#define ret_bayes     4
#define ret_tables    5

/* fields of perfTest and perfTrain */
const char *bayesFields[] = { "TrainError", "LooError", "TestError",  "TrainCM",  "LooCM",  "TestCM" };
const int nBayesFields = sizeof(bayesFields)/sizeof(char *);

/* fields of Tables */
const char *tablesFields[] = { "handle", "UX", "NX", "UY", "NY", "NXY" };
const int nTablesFields = sizeof(tablesFields)/sizeof(char *);

int getDoubleVector(const mxArray *arg, double **x, int *n)
{
  int m = min(mxGetM(arg),mxGetN(arg));
  if (!arg) return -1;  
  *n = m*max(mxGetM(arg),mxGetN(arg));
  if (!mxIsNumeric(arg) ||  mxIsComplex(arg) ||  
      mxIsSparse(arg)  || !mxIsDouble(arg)  || m > 1 || m < 0) {
    *x = NULL;
    return -1;
  }
  *x = mxGetPr(arg); 
  return 0;
}

int getDouble(const mxArray *arg, double *x)
{
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

int getDoubleArray(const mxArray *arg, double **x, int *m, int *n) {
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

void MTree2mxArray(MTree *T, mxArray **U, mxArray **N) {

  int i;
  double *n;
  double *u;

/*    *U = mxCreateNumericMatrix(T->dim, T->nNodes, mxINT32_CLASS, mxREAL); */
  *U = mxCreateDoubleMatrix(T->dim, T->nNodes, mxREAL);
  *N = mxCreateDoubleMatrix(1, T->nNodes, mxREAL);

  n = mxGetPr(*N);
  u = mxGetPr(*U);

  for(i=0; i < T->nNodes; i++) {
    n[i] = T->nodeList[i]->count;
    int2double(T->nodeList[i]->key,u,T->dim);
    u += T->dim;
  }
}

void recJointToFull(JNode *n, double *nxy, int nUX) {
  /* run trough the joint tree in order; that is column by column */ 
  if ( n == 0 ) {
    return;
  } else {
    recJointToFull(n->left,nxy,nUX);
    nxy[n->x+n->y*nUX] = n->count;
    recJointToFull(n->right,nxy,nUX);
  }
}

void recJointToSparse(JNode *n, double *nxy, int *ir, int *jc, int *k, int *c) {
  /* Arguments:
  **   nxy holds the actual values
  **   ir, jc, k and c are bcs. we deal with a sparse matrix
  */
  /* run trough the joint tree in order; that is column by column */ 
  if ( n == 0 ) {
    return;
  } else {
    recJointToSparse(n->left,nxy,ir,jc,k,c);

    /* The following code implements nxy(x,y) = count in the case nxy
    ** is a sparse matrix. See the Matlab docu on the MEX interface
    ** for more info */
    if ( n->y > *c ) {
      /* column changed!! */
      (*c)++;
      jc[*c] = *k;
    }
    nxy[*k] = n->count;
    ir[*k]  = n->x;
    (*k)++;

    recJointToSparse(n->right,nxy,ir,jc,k,c);
  }
}

void jointToSparse(CountDist *D, mxArray *NXY)
{
  int k = 0;
  int c = 0;
  recJointToSparse(D->XYjoint->root,mxGetPr(NXY),mxGetIr(NXY),mxGetJc(NXY),&k,&c);
  mxGetJc(NXY)[D->Ymargin->nNodes] = k;
}

void jointTree2mxArray(CountDist *D, mxArray **NXY)
{
  int nUX = D->Xmargin->nNodes;
  int nUY = D->Ymargin->nNodes;

  if ( (long)nUX * nUY < (long)D->XYjoint->nNodes * 3 ) {
    /* we create a full double matrix */
    *NXY = mxCreateDoubleMatrix(nUX, nUY, mxREAL);
    recJointToFull(D->XYjoint->root,mxGetPr(*NXY),nUX);
  } else {
    /* we save memory by allocating a sparse mxArray */ 
    *NXY = mxCreateSparse(nUX, nUY, D->XYjoint->nNodes, mxREAL);
    jointToSparse(D,*NXY);
  }

}

void writeBayesPerfToMxStruct(mxArray **bayes, CountDist *D, 
                             double *err_train, double *err_loo, double *err_test, int nEval,
                             double *cm_train, double *cm_loo, double *cm_test
                             )
{
  int nUX = D->Xmargin->nNodes;
  mxArray *mxTemp;

  *bayes = mxCreateStructMatrix(1, 1, nBayesFields, bayesFields);

  if ( cm_train ) {
    mxTemp = mxCreateDoubleMatrix(nUX, nUX, mxREAL);
    memcpy(mxGetPr(mxTemp),cm_train,nUX*nUX*sizeof(double));
    mxSetField(*bayes,0,"TrainCM",mxTemp);
    free(cm_train);
  }

  if ( cm_loo ) {
    mxTemp = mxCreateDoubleMatrix(nUX, nUX, mxREAL);
    memcpy(mxGetPr(mxTemp),cm_loo,nUX*nUX*sizeof(double));
    mxSetField(*bayes,0,"LooCM",mxTemp);
    free(cm_loo);
  }

  if ( cm_test ) {
    mxTemp = mxCreateDoubleMatrix(nUX, nUX, mxREAL);
    memcpy(mxGetPr(mxTemp),cm_test,nUX*nUX*sizeof(double));
    mxSetField(*bayes,0,"TestCM",mxTemp);
    free(cm_test);
  }

  if ( err_train ) {
    mxTemp = mxCreateDoubleMatrix(1, nEval, mxREAL);
    memcpy(mxGetPr(mxTemp),err_train,nEval*sizeof(double));
    mxSetField(*bayes,0,"TrainError",mxTemp);
    free(err_train);
  }

  if ( err_loo ) {
    mxTemp = mxCreateDoubleMatrix(1, nEval, mxREAL);
    memcpy(mxGetPr(mxTemp),err_loo,nEval*sizeof(double));
    mxSetField(*bayes,0,"LooError",mxTemp);
    free(err_loo);
  }

  if ( err_test ) {
    mxTemp = mxCreateDoubleMatrix(1, 1, mxREAL);
    memcpy(mxGetPr(mxTemp),err_test,1*sizeof(double));
    mxSetField(*bayes,0,"TestError",mxTemp);
    free(err_test);
  }

}

void writeDistToMxStruct(mxArray **Tables, CountDist *D) {

  mxArray *UX,*NX;
  mxArray *UY,*NY;
  mxArray *NXY;

  *Tables = mxCreateStructMatrix(1, 1, nTablesFields, tablesFields);

  if ( D->handle > -1 ) 
    mxSetField(*Tables,0,"handle",mxCreateScalarDouble(D->handle));
  else
    mxSetField(*Tables,0,"handle",mxCreateDoubleMatrix(0, 0, mxREAL));

  MTree2mxArray(D->Xmargin,&UX,&NX);
  mxSetField(*Tables, 0, "UX", UX);
  mxSetField(*Tables, 0, "NX", NX);

  MTree2mxArray(D->Ymargin,&UY,&NY);
  mxSetField(*Tables, 0, "UY", UY);
  mxSetField(*Tables, 0, "NY", NY);

  jointTree2mxArray(D,&NXY);
  mxSetField(*Tables, 0, "NXY", NXY);

}


/* static variables to manage the different handles and tables */
static CountDist **TheDistArray=0;
static int nDist=0;

CountDist *searchDist(int handle)
/* find a dist with given handle and return that pointer; return NULL
   if handle not found */
{
  int i;
  for(i=0; i<nDist && TheDistArray[i]->handle != handle; i++);
  if ( i == nDist ) 
    return 0;
  else
    return TheDistArray[i];
}

void addDist(CountDist *D)
/* Add dist D to array of distributions */
{
  TheDistArray = (CountDist **)realloc(TheDistArray,(nDist+1)*sizeof(CountDist *));
  TheDistArray[nDist]=D;
  nDist ++;
}

/* Flag to indicate whether we have already registered MexCleanUp */
static int MexInitialized = 0;  

/* Function which cleans up all used memory */
void MexCleanUp(void) {
  int i;
  if ( nDist > 0 ) {
    for(i=0; i<nDist; i++) {
      deleteCountDist(TheDistArray+i);
    }
    if ( TheDistArray ) free(TheDistArray);
    TheDistArray=0;
      mexPrintf("MIBAYES: All stored tables (%i) deleted!\n",nDist);
    nDist = 0;
  }
}

/*
** Here comes the MEX-interface function
*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

  double *X; int dimX; int lenX; /* There are lenX X samples of dimension dimX */
  double *Y; int dimY; int lenY; /* There are lenY Y samples of dimension dimY */

  int    nSamples;  /* if lenX == lenY then nSamples = lenX = lenY */

  double *esz;      /* pointer to indices of evaluatio stops */
  int *evalSize=0;  /* interger version of n with a more descriptive name */
  int nEval;        /* length(n) == number of evaluations */

  double *MI  = 0; /* mutual information */
  double *HX  = 0; /* entropy of X */
  double *HY  = 0; /* entropy of Y */
  double *HXY = 0; /* joint entropy of (X,Y) */

  int evalSize_valid; /* flag that indicates whether the given evalSize's are valid */

  double *err_train=0,*err_loo=0,*err_test=0; /* pointer to arrays for
                                                 bayes errors */

  double *cm_train=0,*cm_loo=0,*cm_test=0;    /* pointer to arrays for
                                                 confusion matrices */

  int distHandle=-1; /* the handle to the distribution */

  double tmp; /* helper variable in several places */


  CountDist *D = 0; /* The actual distributio to work with */

  
  int j; /* loop variable */


  /*** Check if initialized and register MexCleanUp *************************/

  if ( !MexInitialized ) {
    mexAtExit(MexCleanUp);
    MexInitialized = 1;
  }

  /*************** at leat one argument is required **************************/

  if ( nrhs < 1 ) {
    mexErrMsgTxt(usageString);
  }

  /*************** check and get input arguments if only one is given *******/

  /* that is the case: tables = mibayes(handle) */

  if ( nrhs == 1 ) {
    if ( getDouble(prhs[0], &tmp) ) {
      mexErrMsgTxt("MIBAYES-Error: tables = mibayes(handle); Handle is not a scalar!\n");
    }
    distHandle = (int)tmp;
    if ( (D=searchDist(distHandle)) ) {
      writeDistToMxStruct(&(plhs[0]),D);
      return;
    } else {
      mexErrMsgTxt("MIBAYES-Error: tables = mibayes(handle); Invalid handle\n");
    }
  }

  /*************** check and get input arguments if there are 2 or more ****/

  /* 1. argument: the X samples */
  if ( getDoubleArray(argVal(arg_X), &X, &dimX, &lenX) < 0) {
    mexErrMsgTxt(errorString "X ist not a double array!");
  }
  if ( dimX==0 ) {
    mexErrMsgTxt(errorString "Dimension of X data, i.e. size(X,1)==0!");
  }

  /* 2. argument: the Y samples */
  if ( getDoubleArray(argVal(arg_Y), &Y, &dimY, &lenY) < 0) {
    mexErrMsgTxt(errorString "Y ist not a double array!");
  }
  if ( dimY==0 ) {
    mexErrMsgTxt(errorString "Dimension of Y data, i.e. size(Y,1)==0!");
  }

  /* check length of X and Y */
  if ( lenX != lenY ) {
     mexErrMsgTxt(errorString "X and Y have different number of columns!");
  }
  nSamples = lenX;
  if ( nSamples == 0 ) {
    mexPrintf("MIBAYES: No Data supplied. Nothing to do.");
    return;
  }
  
  /*  printf("dimX=%i, dimY=%i, nSamples=%i\n",dimX,dimY,nSamples); */

  /* 3. argument: evaluation sizes */
  evalSize_valid = 0; nEval = 0;
  if ( argGiven(arg_evalSize) ) {
    if ( !mxIsEmpty(argVal(arg_evalSize)) ) {
      /* get esz */
      if ( (getDoubleVector(argVal(arg_evalSize), &esz, &nEval) < 0) || (nEval < 1) ) {
        mexErrMsgTxt(errorString "evalSize ist not a double vector");
      }
      /* make ints from doubles and check if they are increasing */
      evalSize = (int *)mxCalloc(nEval,sizeof(int));
      for(j=0; j<nEval; j++) {
        evalSize[j] = (int)esz[j];
        if ( (j > 0) && (evalSize[j] <=  evalSize[j-1]) ) { 
          mexErrMsgTxt(errorString "evalSize is not monotonically increasing!");
        }
        if ( evalSize[j] > nSamples ||  evalSize[j] < 0 ) {
          mexErrMsgTxt(errorString "All evalSize(j) must be > 0 and <= size(X,2)!");
        }
      }
      evalSize_valid = 1;
    }
  }
  if ( !evalSize_valid ) {
    /* set the default value for n */
    nEval    = 1;
    evalSize = (int *)mxCalloc(nEval,sizeof(int));
    evalSize[0] = nSamples;
  }

  /* 4. argument: handle/index to distribution to work on */
  if ( argGiven(arg_distHandle) ) {
    if ( getDouble(argVal(arg_distHandle), &tmp) ) {
      mexErrMsgTxt(errorString "handle ist not a scalar!");
    }
    if ( (distHandle= (int)tmp) < 0 ) {
      mexErrMsgTxt(errorString "distHanlde must be >= 0!");
    }
     
    if ( (D=searchDist(distHandle)) ) {
      /*       printf("FOUND dist\n"); */
      if ( D->Xmargin->dim != dimX ) {
        mexErrMsgTxt(errorString "Dimension of X data (size(X,1)) and of referenced distribution does not match!");
      }
      if ( D->Ymargin->dim != dimY ) {
        mexErrMsgTxt(errorString "Dimension of Y data (size(Y,1)) and of referenced distribution does not match!");
      }
    } else {
      addDist(D = newCountDist(dimX,dimY,distHandle));
      /*      printf("NEW dist\n"); */
    }
    
  } else {
    /* work on temporary distribution */
    D = newCountDist(dimX,dimY,-1);
    /*    printf("temp dist\n"); */
  }

 
  /**************** output arguments *********************/

  /* 1. Array for mutual information values */
  if ( retGiven(ret_MI) ) {
    retVal(ret_MI) = mxCreateDoubleMatrix(1, nEval, mxREAL);
    MI = mxGetPr(retVal(ret_MI));
  }

  /* 2. array for entropy of X */
  if ( retGiven(ret_HX) ) {
    retVal(ret_HX) = mxCreateDoubleMatrix(1, nEval, mxREAL);
    HX = mxGetPr(retVal(ret_HX));
  }

  /* 3. array for entropy of Y */
  if ( retGiven(ret_HY) ) {
    retVal(ret_HY) = mxCreateDoubleMatrix(1, nEval, mxREAL);
    HY = mxGetPr(retVal(ret_HY));
  }

  /* 4. array for entropy of (X,Y) */
  if ( retGiven(ret_HXY) ) {
    retVal(ret_HXY) = mxCreateDoubleMatrix(1, nEval, mxREAL);
    HXY = mxGetPr(retVal(ret_HXY));
  }

  /* 5. bayes classifier performance */
  if ( retGiven(ret_bayes) ) {
    err_train = (double *)calloc(nEval,sizeof(double));
    err_loo   = (double *)calloc(nEval,sizeof(double));
    err_test  = (double *)calloc(1,sizeof(double));
  }

  /* 6. count tables: nothing to do prior we have some filled tables */

  /************************************************************************************
  ** Finially we are done with the mex interface and can do the real stuff.
  ** See file mi_and_bayes.c for details.
  ************************************************************************************/

  mi_and_bayes(X,Y,nSamples,evalSize,nEval,D,
               MI,HX,HY,HXY,
               err_train,err_loo,err_test,&cm_train,&cm_loo,&cm_test);

  /*********** Fill in return values whos size was not known in advance  *************/

  /*  printf("returning arguments ...\n"); */
  /* fill in leave-one-out performance of Bayes class. */
  if ( retGiven(ret_bayes) ) {
    writeBayesPerfToMxStruct(&(retVal(ret_bayes)),D,
                             err_train, err_loo, err_test, nEval,
                             cm_train, cm_loo, cm_test);
    /* printf("bayes done ...\n"); */
    /*    retVal(ret_bayes) = mxCreateString("hi!"); */
  }

  /* return count tables if required */
  if ( retGiven(ret_tables) ) {
    writeDistToMxStruct(&(retVal(ret_tables)),D);
    /*    printf("tables done ...\n"); */
  }

  /* delete tables if not working on a persistent one */
  if ( argGiven(arg_distHandle) == 0 ) {
    /*    printf("deleting ...\n"); */
    deleteCountDist(&D);
    /*    printf("temp dist deleted ...\n"); */
  }

}

