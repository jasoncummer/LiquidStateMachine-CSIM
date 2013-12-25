#ifndef _RANDGEN_H_
#define _RANDGEN_H_

#include <math.h>
#include <stdlib.h>

extern long IDUM;

double uniform_rand(long *idum);
void rseed(long idum);
double unirnd(void);
double normrnd(void);

#endif
