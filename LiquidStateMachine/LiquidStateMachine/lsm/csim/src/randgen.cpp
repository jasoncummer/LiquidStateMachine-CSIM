/*! \file randgen.cpp
**  \brief Implementation of some pseudo random number generators.
**
**  The core is the function uniform_rand() which is taken from 
**  Numerical Recipies in C. It is used to derive the normal 
**  distributed generator normrnd().
*/

#include "randgen.h"


#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

long IDUM;

/* uniform random number generator */
double uniform_rand(long *idum)

     /*! Long period (> 2 10 18 ) random number generator of L'Ecuyer
     ** with Bays-Durham shuffle and added safeguards. Returns a uniform
     ** random deviate between 0.0 and 1.0 (exclusive of the endpoint
     ** values). Call with idum a negative integer to initialize;
     ** thereafter, do not alter idum between successive deviates in a
     ** sequence. RNMX should approximate the largest floating value
     ** that is less than 1.  */

     /* (C) Copr. 1986-92 Numerical Recipes Software )'). */

{
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	double temp;

	if (*idum <= 0) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		idum2=(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum < 0) *idum += IM1;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ1;
	*idum=IA1*(*idum-k*IQ1)-k*IR1;
	if (*idum < 0) *idum += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ( (temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

void rseed(long idum)
/*! Set the seed of the random number generator. */
{
  IDUM = -labs(idum);
  uniform_rand(&IDUM);
}

double unirnd(void)
/*! Returns a random number from the interval (0,1). */
{
  return uniform_rand(&IDUM);
}

double normrnd(void)
/*! Gaussion random variable with zero mean and variace 1.0.
**  Taken from Numerical Cecipies in C.
*/
{
  static int iset=0;
  static double gset;
  double fac,rsq,v1,v2;

  if  (iset == 0) {
    do {
      v1=2.0*unirnd()-1.0;
      v2=2.0*unirnd()-1.0;
      rsq=v1*v1+v2*v2;
    } while (rsq >= 1.0 || rsq == 0.0);
    fac=sqrt(-2.0*log(rsq)/rsq);
    gset=v1*fac;
    iset=1;
    return v2*fac;
  } else {
    iset=0;
    return gset;
  }
}

#ifdef _TEST_RND_GEN_
main(void)
{
  long int i;
  rseed(clock());
  for(i=0;i<100000;i++)
    printf("%g %g\n",unirnd(),normrnd());
}
#endif

