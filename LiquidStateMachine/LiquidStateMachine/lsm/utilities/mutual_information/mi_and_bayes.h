#ifndef __MI_BAYES_H__
#define __MI_BAYES_H__

typedef struct dist_tag CountDist; /* Structure implementing the observed distributions */

typedef struct mnode_tag MNode; /* node for the tree implementing the MARGIN distributions */
typedef struct mtree_tag MTree; /* tree implementing the MARGIN distributions */
typedef struct jnode_tag JNode; /* node for the tree implementing the JOINT distributions */
typedef struct jtree_tag JTree; /* tree implementing the JOINT distributions */

/*
** The full observed distributions/tables
*/
struct dist_tag {
  MTree *Xmargin;  /* the margin distribution of x values */
  MTree *Ymargin;  /* the margin distribution of y values */
  JTree *XYjoint;  /* the joint distribution of pairs (x,y) */
  int handle;      /* ID of that distribution */
};

/*
** Node for the MARGIN ditributions/tables
*/
struct mnode_tag {
  int   idx;      /* unique index for this key */
  int   count;    /* how often this key occurs in the data */
  int   *key;     /* the key / value */
  JNode *joint;   /* root of a list to joint nodes; i.e. the counts conditioned on the key/value */
                  /* the margin distribution is implemented as binary search tree */
  MNode  *left;    /* left subtree for keys smaller than this key */
  MNode  *right;   /* right subtree for keys larger than this key */
};
 

/*
** Tree implementing the MARGIN distribution/table
*/
struct mtree_tag {
  int nNodes;        /* number of elements in the; i.e. number of distict samples observed */
  int dim;           /* dimension of keys/values stored */
  int nSamples;      /* total numer of samples seen so far; i.e. nSamples = SUM nodes->count */
  MNode *root;       /* root node of the tree; node with index 0 */
  MNode **nodeList;  /* a list of pointer to the nodes of the tree; used for indexed access to the nodes */
  int nAlloc;        /* number of elemets currently allocated for nodeList */
};

/*
** node for the JOINT ditribution/table of pairs (x,y)
*/
struct jnode_tag {
  int x;        /* index of observed x; the key/value is Xmargin->nodeList[x]->key */
  int y;        /* index of observed y; the key/value is Ymargin->nodeList[y]->key */
  int count;    /* count of how often the pair (x,y) was obsered in the data */

                /* The joint distribution is implemented as binary
                ** search tree. The tree is sorted by y first and then
                ** by x. The reason for this is that this allows a
                ** fast conversion of the tree into a spares Matlab
                ** matrix */
  JNode *left;  /* left  subtree for pairs (x',y') smaller than (x,y) */
  JNode *right; /* right subtree for pairs (x',y') larger than (x,y) */
  JNode *nextx; /* points to the next joint node with the same y;
                   i.e. by following this links one can traverse
                   trough the counts conditioned on y */
  JNode *nexty; /* points to the next joint node with the same x;
                   i.e. by following this links one can traverse
                   trough the counts conditioned on x */
};
 
/*
** Tree implementing the JOINT distribution
*/
struct jtree_tag {
  int nNodes;     /* number of nodes; i.e. number of distince pairs (x,y) observed */
  int nSamples;   /* total numer of samples seen so far; i.e. nSamples = SUM nodes->count */
  JNode *root;    /* root node of the tree */
};

void double2int(double *d, int *i, int dim);
void int2double(int *i, double *d, int dim);

/* Create a new counting table/distribution */
CountDist *newCountDist(int dimX, int dimY, int handle);

/* Delete all internal memory of a counting table/distribution */
void deleteCountDist(CountDist **D);

/* Calculate mutual information, entropies and Bayes errors for data X
   and Y given the initial counting tables D. For details on the
   arguments see mi_and_bayes.c */
int mi_and_bayes(double *X, double *Y, int nNewSamples, int *evalSize, int nEval, CountDist *D,
                  double *MI, double *HX, double *HY, double *HXY, 
                  double *err_train, double *err_loo, double *err_test, double **cm_train, double **cm_loo, double **cm_test);

#endif
