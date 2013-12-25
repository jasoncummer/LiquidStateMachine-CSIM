/** \file csimmex.h
 ** \brief Declares all csimMex<command> functions.
 */
#ifndef _CSIMMEX_H_
#define _CSIMMEX_H_

#include <mex.h>
#include "globaldefinitions.h"

int csimMexInit(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexCreate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexSet(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexGet(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexConnect(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexReset(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexSimulate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexExport(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexImport(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexDestroy(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
int csimMexList(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
void csimMexCleanUp(void);

int getDouble(const mxArray *arg, double *x);
int getDoubleArray(const mxArray *arg, double **x, int *m, int *n);
int getDoubleVector(const mxArray *arg, double **x, int *n);
int getUint32Vector(const mxArray *arg, uint32 **x, int *n);
int getString(const mxArray *arg, char **str);

class MexNetwork;
extern MexNetwork *TheNetwork;

extern int MAXINTEGER;

#endif
