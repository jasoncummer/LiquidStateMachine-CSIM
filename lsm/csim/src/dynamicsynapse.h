
//! The use parameter of the dynamic synapse [range=(1e-5,1); units=;]
float U;

//! The time constant of the depression of the dynamic synapse [range=(0,10); units=sec;]
float D;

//! The time constant of the facilitation of the dynamic synapse [range=(0,10); units=sec;]
float F;

//! Value of the time varying facilitation state variable \f$u\f$  for the first spike [range=(0,1); units=;]
float u0;

//! Value of the time varying depression state variable \f$r\f$ for the first spike [range=(0,1); units=;]
float r0;              

//! The time varying state variable \f$u\f$ for facilitation \internal [readonly; units=; range=(0,1);]
double u;

//! The time varying state variable \f$u\f$ for depression  \internal [readonly; units=; range=(0,1);]
double r;              

//! The last spike time \internal [hidden;]
double lastSpike;

#define DYNAMIC_PSR_CHANGE { \
  if ( lastSpike > 0 ) { \
    double isi = SimulationTime - lastSpike; \
    r = 1 + (r*(1-u)-1)*exp(-isi/D); \
    u = U + u*(1-U)*exp(-isi/F); \
  } \
  psr += ((W/decay) * u * r); \
  lastSpike = SimulationTime; \
}

