
#include <math.h>
#include <mex.h>
#include "mi_and_bayes.h"

MNode *newMNode(MTree *M, int *key) 
     /* Init a node with key for a binary tree M representing a margin
        distribution */
{
  MNode *m = (MNode *)malloc(1*sizeof(MNode));

  m->count = 1;
  m->key   = malloc(M->dim*sizeof(int));
  memcpy(m->key,key,M->dim*sizeof(int));
  m->idx   = (M->nNodes)++;

  m->left = m->right = 0;
  m->joint = 0;

  if ( M->nNodes > M->nAlloc ) {
    M->nAlloc *= 2;
    M->nodeList = (MNode **)realloc(M->nodeList,M->nAlloc*sizeof(MNode *));
  }
  M->nodeList[m->idx] = m;

  M->nSamples++;

  return m;
}

void deleteMNode(MNode **m)
     /* Pretty obvious, isn't it? */
{
  if ( (*m)->key ) free((*m)->key);
  (*m)->key=0;
  if ( *m ) free(*m);
  *m = NULL;
}

MTree *newMTree(int dim)
     /* Allocate mamory and init a new tree representing a margin
        distribution */
{
  MTree *M = (MTree *)malloc(1*sizeof(MTree));

  M->root     = 0;
  M->nNodes   = 0;
  M->dim      = dim;
  M->nSamples = 0;

  M->nAlloc   = 2;
  M->nodeList = (MNode **)realloc(0,M->nAlloc*sizeof(MNode *));

  return M;
}

void recDeleteMNodes(MNode *m)
     /* Recursively delete all nodes in the tree rooted a m */
{
  if ( m == 0 )
    return;
  else {
    recDeleteMNodes(m->left);
    recDeleteMNodes(m->right);
    deleteMNode(&m);
  }
}

void deleteMTree(MTree **M)
     /* Use recDeleteMNodes to delete a binary tree and also delete
        the nodeList */
{
  if ( *M ) {
    recDeleteMNodes((*M)->root);
    if ( (*M)->nodeList ) free((*M)->nodeList);
    (*M)->nodeList = 0;
    free(*M);
    *M = NULL;
  }
}


void double2int(double *d, int *i, int dim)
{
  int k;
  for(k=0; k<dim; k++) i[k] = d[k];
}

void int2double(int *i, double *d, int dim)
{
  int k;
  for(k=0; k<dim; k++) d[k] = i[k];
}

int veccmp(int *a, int *b, int d)
     /* 
     ** Compares the two integer vectors a and b.
     ** Returns -1 if a < b
     **           0 if a == b
     **           +1 if a > b
     */
{
  int i;

  for(i=0; i<d && a[i]==b[i]; i++);

  /* all 'digits' are equal; hence 'a == b' */
  if ( i == d ) return 0;

  /* all 'digits' up to i are equal; if a[i] < b[i] then 'a < b' */
  if ( a[i] < b[i] ) return -1;

  /* only the possibility 'b > a' remains  */
  return +1;
}

int recMarginIncrement(MTree *M, MNode **p, int *key)
     /* Search a node with key/value key.  If found increments
     ** count. If not found add new node to tree. */
{
  register int cmp;
  register MNode *m=*p;

  if ( m == 0 ) {
    /* key not found; add new MNode */
    (*p)=newMNode(M,key);
    return (*p)->idx;
  } else {
    if ( (cmp=veccmp(key,m->key,M->dim)) < 0 ) {
      /* key is smaller then m->key, hence go into left subtree */
      return recMarginIncrement(M,&(m->left),key);
    } else if ( cmp > 0 ) {
      /* key is larger then m->key, hence go into right subtree */
      return recMarginIncrement(M,&(m->right),key);
    } else {
      /* key found! */
      m->count++;
      M->nSamples++;
      return m->idx;
    }
  }
}


int marginIncrement(MTree *M, int *key)
/* just hide the recursive implementation */
{
  return recMarginIncrement(M,&(M->root),key);
}

int recMarginSearch(MTree *M, MNode *m, int *key)
     /* Search a node with key/value key.  If found increments
     ** count. If not found add new node to tree. */
{
  register int cmp;

  if ( m == 0 ) {
    /* key not found */
    return -1;
  } else {
    if ( (cmp=veccmp(key,m->key,M->dim)) < 0 ) {
      /* key is smaller then m->key, hence go into left subtree */
      return recMarginSearch(M,m->left,key);
    } else if ( cmp > 0 ) {
      /* key is larger then m->key, hence go into right subtree */
      return recMarginSearch(M,m->right,key);
    } else {
      /* key found! */
      return m->idx;
    }
  }
}


int marginSearch(MTree *M, int *key)
/* just hide the recursive implementation */
{
  return recMarginSearch(M,M->root,key);
}

void recMarginEntropy(MNode *m, double *h)
/* recursive version of SUM_OVER_n n*log(n) */
{
  if ( m == 0 ) {
    return;
  } else {
    recMarginEntropy(m->left,h);
    (*h) += (double)m->count*log(m->count);
    recMarginEntropy(m->right,h);
  }
}

double marginEntropy(MTree *M)
/* H = ld(nSamples) - 1/nSamples * SUM_OVER_n n*ld(n)
** where n are all nonzero counts */
{
  double H = 0.0;

  /* this does the sum over all n */
  recMarginEntropy(M->root,&H);

  /* complete the formula */
  return (-H/(M->nSamples)+log(M->nSamples))/log(2);
}


/************************** JOINT Nodes, JOINT Tree ************************/

JNode *newJNode(CountDist *D, int x, int y) 
{
  register MNode *xm,*ym;
  JNode *xy = (JNode *)malloc(1*sizeof(JNode));

  D->XYjoint->nNodes++;

  xy->count = 1;
  xy->x = x;
  xy->y = y;
  xy->left  = xy->right = 0;

  /* insert node into conditional on y */
  xy->nextx = (ym=(D->Ymargin->nodeList[y]))->joint; ym->joint = xy;

  /* insert node into conditional on x */
  xy->nexty = (xm=(D->Xmargin->nodeList[x]))->joint; xm->joint = xy;

  D->XYjoint->nSamples++;

  return xy;
}

void deleteJNode(JNode **xy)
{
  if ( *xy ) free(*xy);
  *xy = NULL;
}

JTree *newJTree(void)
{
  JTree *J = (JTree *)malloc(1*sizeof(JTree));

  J->root     = 0;
  J->nNodes   = 0;
  J->nSamples = 0;
  return J;
}

void recDeleteJNodes(JNode *xy)
{
  if ( xy == 0 )
    return;
  else {
    recDeleteJNodes(xy->left);
    recDeleteJNodes(xy->right);
    deleteJNode(&xy);
  }
}

void deleteJTree(JTree **M)
{
  if ( *M ) {
    recDeleteJNodes((*M)->root);
    free(*M);
    *M = NULL;
  }
}

int idxcmp(int i1, int j1, int i2, int j2)
     /* 
     ** Compares the two tuples (i1,j1) and (i2,j2)
     **
     ** Returns -1 if  (i1,j1) < (i2,j2)
     **           0 if (i1,j1) == (i2,j2)
     **           +1 if (i1,j1) > (i2,j2)
     **
     ** First the j's are compared; if equal the the i's are used.  */
{
  if ( j1 == j2 ) {
    if ( i1 == i2 )
      return 0;
    else if ( i1 < i2 )
      return -1;
    else
      return +1;
  } else if ( j1 < j2 )
    return -1;
  else
    return +1;
 
}

void recJointIncrement(CountDist *D, JNode **p, int x, int y)
     /* Search a node with index pair (x,y).  If found increments
     ** count. If not found add new node to tree. */
{
  register int cmp;
  register JNode *xy=*p;

  if ( xy == 0 ) {
    /* pair (x,y) not found! */
    (*p)=newJNode(D,x,y);
  } else {
    if ( (cmp=idxcmp(x,y,xy->x,xy->y)) < 0 ) {
      /* (x,y) is smaller then (xy->x,xy->y), hence go into left subtree */
      recJointIncrement(D,&(xy->left),x,y);
    } else if ( cmp > 0 ) {
      /* (x,y) is larger then (xy->x,xy->y), hence go into left subtree */
      recJointIncrement(D,&(xy->right),x,y);
    } else {
      /* key found! */
      xy->count++;
      D->XYjoint->nSamples++;
    }
  }
}

void jointIncrement(CountDist *D, int x, int y)
{
  recJointIncrement(D,&(D->XYjoint->root),x,y);
}

void recJointEntropy(JNode *n, double *h) {
  if ( n == 0 ) {
    return;
  } else {
    (*h) += (double)n->count*log(n->count);
    recJointEntropy(n->left,h);
    recJointEntropy(n->right,h);
  }
}

double jointEntropy(JTree *J)
{
  double h = 0.0;
  recJointEntropy(J->root,&h);
  return (-h/(J->nSamples)+log(J->nSamples))/log(2);
}

CountDist *newCountDist(int dimX, int dimY, int handle) 
{
  CountDist *D = (CountDist *)malloc(1*sizeof(CountDist));

  D->Xmargin = newMTree(dimX);
  D->Ymargin = newMTree(dimY);
  D->XYjoint = newJTree();
  
  D->handle = handle;
  return D;
}

void deleteCountDist(CountDist **D)
{
  if ( *D ) {
    deleteMTree(&((*D)->Xmargin));
    deleteMTree(&((*D)->Ymargin));
    deleteJTree(&((*D)->XYjoint));
    free(*D);
    *D = NULL;
  }
}

void findMAPclassification(CountDist *D, int yidx, int *class, int *MAPcount)
{
  int mapx=-1;
  register int mapcount=-1;
  register JNode *xy;
  
  for(xy=D->Ymargin->nodeList[yidx]->joint; xy != 0; xy = xy->nextx) {
    if ( xy->count > mapcount ) {
      mapx     = xy->x;
      mapcount = xy->count;
    }
  }
  if ( mapx == -1 ) {
    printf("This should really not happen!!\n");
  }
  *class     = mapx;
  *MAPcount = mapcount;
}

void trainBayes(double *X, double *Y, int from, int to, CountDist *D)
{

  int k; /* loop variables */

  int dimX = D->Xmargin->dim;
  int dimY = D->Ymargin->dim;

  int *xint = (int *)malloc(dimX*sizeof(int));
  int *yint = (int *)malloc(dimY*sizeof(int));

  int xidx,yidx;

  for(k=from; k<to; k++) {

    double2int(X+k*dimX,xint,dimX);
    double2int(Y+k*dimY,yint,dimY);
   
    xidx = marginIncrement(D->Xmargin,xint);
    yidx = marginIncrement(D->Ymargin,yint);

    jointIncrement(D,xidx,yidx);

  }

  free(xint);
  free(yint);

}

double testBayesError(double *X, double *Y, int from, int to, CountDist *D, double *CM)
{
  double err = 0.0; /* total error count */

  int class;    /* classification of yidx */
  int count;    /* joint count of maximum aposterior hypothesis for x given yidx */
  int nCM = D->Xmargin->nNodes; /* just renaming; size of confusion matrix CM */

  int i,k; /* loop variables */

  int dimX = D->Xmargin->dim;
  int dimY = D->Ymargin->dim;

  int *xint = (int *)malloc(dimX*sizeof(int));
  int *yint = (int *)malloc(dimY*sizeof(int));

  int xidx,yidx;
  /* init confusion matrix CM */
  for(i=0; i<nCM*nCM; i++) CM[i] = 0.0;

  for(k=from; k<to; k++) {

    double2int(X+k*dimX,xint,dimX);
    double2int(Y+k*dimY,yint,dimY);

    yidx = marginSearch(D->Ymargin,yint);

    if ( yidx > -1 ) {
      /* y found! I.e. it was already observed, so we find the MAP
      ** classification for yidx and compare it with the actual x. */
      findMAPclassification(D,yidx,&class,&count);
    } else {
      /* y NOT found! Hence we have to guess a class for yidx. We
      ** just use the one with the highest index. */
      class = D->Xmargin->nNodes-1;
    }

    xidx = marginSearch(D->Xmargin,xint);

    if ( xidx > -1 ) {
      if ( class != xidx ) {
        err += 1.0;
      }
      /* 
      ** We only add known classes to the confusion matrix!!!!  Hence
      ** CM is only correct/usefull if all x in the test set have
      ** already been observed ones! */
      CM[xidx+class*nCM] += 1.0;
    } else {
      /* Note that in the case xidx == -1 (i.e. x was not observed yet)
      ** we count an error. */
      err += 1.0;
    }

  }

  free(xint);
  free(yint);

  return err / ( to - from );
}

double trainBayesError(CountDist *D, double *CM)
{
  register int yidx; /* loop variable over all observat y samples */
  register JNode *xy;  /* loop variable over the conditional on yidx */

  double err = 0.0; /* total error count */

  int class;     /* maximum aposterior hypothesis for x given yidx */
  int count; /* joint count of maximum aposterior hypothesis for x given yidx */
  int nCM = D->Xmargin->nNodes; /* just renaming */

  int i;
  /* init confusion matrix */
  for(i=0; i<nCM*nCM; i++)
    CM[i] = 0.0;

  for(yidx=0; yidx < D->Ymargin->nNodes; yidx++) {

    findMAPclassification(D,yidx,&class,&count);
    err += (D->Ymargin->nodeList[yidx]->count - count);

    /* fill confusion matrix CM. CM is nCM x nCM */
    for(xy=D->Ymargin->nodeList[yidx]->joint; xy != 0; xy = xy->nextx) {
      CM[xy->x+class*nCM] += xy->count;
    }  
  }

  return err / D->XYjoint->nSamples;
}

double leaveOneOutBayesError(CountDist *D, double *CM)
{
  int    yidx;  /* loop variable over all observat y samples */
  double err;   /* total error count */

  int class;     /* maximum aposterior hypothesis for x given yidx */
  int count; /* joint count of maximum aposterior hypothesis for x given yidx */
  
  JNode *xy;    /* loop variable over the conditional on yidx */
  MNode *ym;    /* helper variable */

  int nCM = D->Xmargin->nNodes; /* just renaming */
  int i;
  /* init confusion matrix */
  for(i=0; i<nCM*nCM; i++)
    CM[i] = 0.0;

  err = 0.0;
  for(yidx=0; yidx < D->Ymargin->nNodes; yidx++) {
    ym = D->Ymargin->nodeList[yidx];
    for(xy=ym->joint; xy != 0; xy = xy->nextx) {

      if ( ym->count > 1 ) {
        xy->count--;
        findMAPclassification(D,yidx,&class,&count);
        if ( xy->x != class ) {
          err += (xy->count+1);
        }
        xy->count++; 
      } else {
        /* any guess for class is ok */
        class = D->Xmargin->nNodes-1;
        if ( xy->x != class ) {
          err += (xy->count);
        }
      }

      CM[xy->x+class*nCM] += xy->count;

    }
  }
  return err / D->XYjoint->nSamples;
}

int mi_and_bayes(double *X, double *Y, int nNewSamples, int *evalSize, int nEval, CountDist *D,
                  double *MI, double *HX, double *HY, double *HXY, 
                  double *err_train, double *err_loo, double *err_test, double **cm_train, double **cm_loo, double **cm_test)
/* Calculate mutual information, entropies and Bayes errors for data X
** and Y given the initial counting tables D.
** 
** Input Arguments:
**
**   X ... pointer to an array of x-samples; dimensions are
**   D->Xmargin->dim x N where is implizitly given in evalSize
**
**   Y ... pointer to an array of y-samples; dimensions are
**   D->Ymargin->dim x N where is implizitly given in evalSize 
**   
**   evalSize ... pointer to sizes at which to evaluate the count
**   distributions so far. The samples evalSize[i] to
**   nNewSamples are used for testing.
**
**   nEval ... how many entries evalSize has
**
**   D ... count distribution/tables to use
**
** Return Arguments:
**
**   MI ... pointer to array of size nEval to store calculated mutual
**   information values
**
**   HX ... pointer to array of size nEval to store calculated
**   entropies for the x-samples.
**
**   HY ... pointer to array of size nEval to store calculated
**   entropies for the y-samples.
**
**   HXY ... pointer to array of size nEval to store calculated
**   joint entropies.
**
**   err_train ... pointer to array of size nEval to store calculated
**   error of the Bayes classifier on the training data. Nothing is
**   returned if err_train == 0.
**
**   err_loo ... pointer to array of size nEval to store calculated
**   leave-one-out errors of the Bayes classifier. Nothing is returned
**   if err_loo == 0.
**
**   cm_train, cm_loo ... confusion matrix of Bayes classifier on
**   train set and for leave-one-out. mi_and_bayes allocates memory to
**   hold this matrices since the size is not known in advance.  */

{
  int iEval; /* loop variables */

  int nUX; /* num. of unique x-samples */
  
  double mi,hx,hy,hxy;

  *cm_train = 0;
  *cm_loo  = 0;
  *cm_test  = 0;
  
  for(iEval=0; iEval < nEval; iEval++ ) {
  
    /*
    ** LEARNING: i.e. add samples to the count tables;
    */
    if ( iEval == 0 )
      trainBayes(X,Y,0,evalSize[iEval],D);
    else
      trainBayes(X,Y,evalSize[iEval-1],evalSize[iEval],D);

    /*
    ** evaluate MUTUAL INFORMATION on data seen so far
    */
    hx  = marginEntropy(D->Xmargin);
    hy  = marginEntropy(D->Ymargin);
    hxy = jointEntropy(D->XYjoint);
    mi  = hx + hy - hxy;
    
    /* retrun the calculated values if the pointers are non-nil */
    if (MI)  MI[iEval]  = mi;
    if (HX)  HX[iEval]  = hx;
    if (HY)  HY[iEval]  = hy;
    if (HXY) HXY[iEval] = hxy;
    
    /*
    ** evaluate empirical Bayes classifier on data seen so far
    */
    nUX = D->Xmargin->nNodes; /* number of unique x-samples */
    if ( nUX > 0 ) {

      /* TRAIN ERROR */
      if ( err_train  ) {
        *cm_train = (double *)realloc(*cm_train,nUX*nUX*sizeof(double));
        if ( *cm_train ) {
          err_train[iEval] = trainBayesError(D,*cm_train);
        } else {
          mexPrintf("MIBAYES-Error: could not alloc memory for train confusion matrix!\n");
          return -1;
        }
      }
      
      /* LEAVE ONE OUT ERROR */
      if ( err_loo ) {
        *cm_loo = (double *)realloc(*cm_loo,nUX*nUX*sizeof(double));
        if ( *cm_loo ) {
          err_loo[iEval]  = leaveOneOutBayesError(D,*cm_loo);
        } else {
          mexPrintf("MIBAYES-Error: could not alloc memory for leave-one-out confusion matrix!\n");
          return -1;
        }
      }
    } 

    if ( nEval > 1 ) {
      mexPrintf("N=%i (nUX=%i, nUY=%i): MI=%g (hx=%g hy=%g hxy=%g); bayes-errors: ",
                D->Xmargin->nSamples,D->Xmargin->nNodes,D->Ymargin->nNodes,mi,hx,hy,hxy);
      if ( err_train ) mexPrintf("train=%g ",err_train[iEval]);
      if ( err_loo ) mexPrintf("loo=%g ",err_loo[iEval]);
      mexPrintf("\n");
    }
    
  }

  /*
  ** TESTING: Test the remaining samples on the current count tables
  */
  if ( err_test && ( nNewSamples-evalSize[nEval-1] > 0 ) ) {
    nUX = D->Xmargin->nNodes; /* number of unique x-samples */
    *cm_test  = (double *)realloc(*cm_test,nUX*nUX*sizeof(double));
    if ( *cm_test ) {
      *err_test = testBayesError(X,Y,evalSize[nEval-1],nNewSamples,D,*cm_test);
      /* mexPrintf("MIBAYES: error on test set = %g (%i samples)\n",*err_test,nNewSamples-evalSize[nEval-1]); */
    } else {
      mexPrintf("MIBAYES-Error: could not alloc memory for test confusion matrix!\n");
      return -1;
    }
  }

  return 0;

}





