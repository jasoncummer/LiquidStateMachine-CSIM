/** \file armmodel.cpp
**  \brief Arm model (for Prashant Joshi).
*/

#include "armmodel.h"
#include <math.h>
#include <mat.h>

// constant PI
const double PI = acos(-1.0);


/** Constructs a new arm model. This model has 2 inputs and 300 outputs. */

ArmModel::ArmModel() : PhysicalModel(2, 300) {

  // First you have to register the names of your input- and output-channels
  // (for the connect command)

  register_input_channel("torque1");  // index 0
  register_input_channel("torque2");  // index 1

  // Register all 300 output channels
  char outputname[10];
  int i;
  for (i=0; i<50; i++) {
    sprintf(outputname, "Xdest_%d", i);
    register_output_channel(outputname); // index 0-49
  }
  for (i=0; i<50; i++) {
    sprintf(outputname, "Ydest_%d", i);
    register_output_channel(outputname); // index 50-99
  }
  for (i=0; i<50; i++) {
    sprintf(outputname, "T1_%d", i);
    register_output_channel(outputname); // index 100-149
  }
  for (i=0; i<50; i++) {
    sprintf(outputname, "T2_%d", i);
    register_output_channel(outputname); // index 150-199
  }
  for (i=0; i<50; i++) {
    sprintf(outputname, "U1_%d", i);
    register_output_channel(outputname); // index 200-249
  }
  for (i=0; i<50; i++) {
    sprintf(outputname, "U2_%d", i);
    register_output_channel(outputname); // index 250-299
  }


  // Insert Better starting values !!!!
  w1 = 0;
  w2 = 0;

  u1 = 0;
  u2 = 0;

  t1 = 0;
  t2 = 0;

  //! The time-step of simulation.
  model_DT = 0.002;

  //! The X coordinate of origin
  Xo = 1;

  //! The Y coordinate of origin
  Yo = 1;

  //! The mass of 1st link
  m1 = 1;

  //! The mass of second link
  m2 = 1;

  //! The distance of central point of link 1.
  lc1 = 0.25;

  //! The distance of central point of link 2.
  lc2 = 0.25;

  //! Length of first link
  l1 = 0.5;

  //! Length of second link
  l2 = 0.5;

  //! The moment of inertia of link1.
  I1 = 0.03;

  //! The moment of inertia of link2.
  I2 = 0.03;

  //! The factor by which the external noise will be  proportional to the input magnitude.
  MFACT = 1e-2;

  //! The time at which the perturbation will start
  PERT_TIME = 0;

  //! The duration of perturbation
  DURATION = 0;


  // File numbers (for input-file)
  inputFileNr = 0;
  oldInputFileNr = -1;

  TIMESTEPS = 0;

  cur_timestep = 0;

  nr_file_steps = (int) (model_DT / 1e-4);

  sim_step_counter = 0;


  Xdest = 0;
  Ydest = 0;
  theta1 = 0;
  theta2 = 0;
  nu1 = 0;
  nu2 = 0;
  c_theta1 = 0;
  c_theta2 = 0;
  c_u1 = 0;
  c_u2 = 0;

  // Create an output-buffer
  buffer_length = 2;
  buffer_position = 0;
  out_buffer = new double* [300];
  for (i=0; i<300; i++)
    out_buffer[i] = new double[buffer_length];
}

/** Frees the memory. */
ArmModel::~ArmModel() {
  if (Xdest)
    free(Xdest);
  Xdest = 0;
  if (Ydest)
    free(Ydest);
  Ydest = 0;
  if (theta1)
    free(theta1);
  theta1 = 0;
  if (theta2)
    free(theta2);
  theta2 = 0;
  if (nu1)
    free(nu1);
  nu1 = 0;
  if (nu2)
    free(nu2);
  nu2 = 0;
  if (c_theta1)
    free(c_theta1);
  c_theta1 = 0;
  if (c_theta2)
    free(c_theta2);
  c_theta2 = 0;
  if (c_u1)
    free(c_u1);
  c_u1 = 0;
  if (c_u2)
    free(c_u2);
  c_u2 = 0;

  if (out_buffer) {
    for (int i=0; i<300; i++)
      delete[] out_buffer[i];
    delete[] out_buffer;
  }
  out_buffer = 0;
}

/** Transforms the current inputs to new output values.
    \param I Array of pointers to input values from the readouts.
    \param O Array of output values.
    \return -1 if an error occured, 1 for success. */
int ArmModel::transform(double** I, double* O) {

  int i;

  // Watch if a time-step change occurs
  if (++sim_step_counter >= nr_file_steps) {
    cur_timestep++;
    sim_step_counter = 0;
  }

  if (cur_timestep < 1) {
    return 1;
  }

  // Get input values
  double ns_u1 = *(I[0]);  // equivalent to ns_u1 = *(I[ input_channel_names["torque1"] ])
  double ns_u2 = *(I[1]);  // equivalent to ns_u2 = *(I[ input_channel_names["torque2"] ])


  /* *******************************************
     Calculate Transformation
     ******************************************* */

  // Define the Inertia Matrix elements.
  double H11 = m1*lc1*lc1 + I1 + m2*(l1*l1 + lc2*lc2 + 2*l1*lc2*cos(t2)) + I2;
  double H12 = m2*l1*lc2*cos(t2) + m2*lc2*lc2 + I2;
  double H21 = H12;
  double H22 = m2*lc2*lc2 + I2;

  // Define h
  double h = m2*l1*lc2*sin(t2);

  // Determinant
  double det = H11*H22 - H12*H21;

  // Compute the inverse matrix of H
  double InvH11 = H22 / det;
  double InvH12 = -H12 / det;
  double InvH21 = -H21 / det;
  double InvH22 = H11 / det;

  // Make the torque vector.
  //  T = [u1 u2]'; '

  // Compute the Centripetal and Coriolis force matrix.
  double c11 = -h*w2;
  double c12 = -h*(w1+w2);
  double c21 = h*w1;
  // double c22 = 0;

  // Make the Omega vector.
  // OMEGA = [w1 w2]'; '

  // Now compute the ALPHA
  // ALPHA = IH*(T - C*OMEGA);

  double help1 = u1 - (c11*w1 + c12*w2);
  double help2 = u2 - c21*w1;
  double alpha1 = InvH11 * help1 + InvH12 * help2;
  double alpha2 = InvH21 * help1 + InvH22 * help2;
  //  csimPrintf("Help : %g       %g             %g\n", help1, help2, det);
  //  csimPrintf("Alpha: %g       %g\n", alpha1, alpha2);

  // Now start calculating the next step traj elems.

  //  ns_w1 = ALPHA(1)*DT + w1;
  //  ns_w2 = ALPHA(2)*DT + w2;
  double ns_w1 = w1 + alpha1*DT;
  double ns_w2 = w2 + alpha2*DT;

  // ns_t1 = w1*DT + t1;
  // ns_t2 = w2*DT + t2;
  double ns_t1 = w1*DT + t1;
  double ns_t2 = w2*DT + t2;


  //  csimPrintf("New Omega : %g        %g\n", ns_w1, ns_w2);
  //  csimPrintf("New Torque: %g        %g\n", ns_u1, ns_u2);
  //  csimPrintf("New Theta : %g        %g\n", ns_t1, ns_t2);



  /* *******************************************
     MAKE OUTPUT DISTRIBUTION
     ******************************************* */

  // Calculate next position in circular buffer
  int next_buffer_position = (buffer_position+1) % buffer_length;

  // without any interpolation between time-steps

  dist_OP(ns_t1, 100, next_buffer_position, cur_timestep+1, "c_theta1");
  dist_OP(ns_t2, 150, next_buffer_position, cur_timestep+1, "c_theta2");

  // WHY DO YOU ADD 10 TO ns_u1, ns_u2 ???
  dist_OP(ns_u1+10, 200, next_buffer_position, cur_timestep+1, "c_u1");
  dist_OP(ns_u2+10, 250, next_buffer_position, cur_timestep+1, "c_u2");

  int pos = cur_timestep * 50;

  for (i=0; i<50; i++) {
    out_buffer[i][next_buffer_position] = Xdest[pos+i];
    out_buffer[i+50][next_buffer_position] = Ydest[pos+i];

    if (out_buffer[i+100][next_buffer_position] != 0)
      out_buffer[i+100][next_buffer_position] += mintheta1;
    if (out_buffer[i+150][next_buffer_position] != 0)
      out_buffer[i+150][next_buffer_position] += mintheta2;

    if (out_buffer[i+200][next_buffer_position] != 0)
      out_buffer[i+200][next_buffer_position] += minU1;
    if (out_buffer[i+250][next_buffer_position] != 0)
      out_buffer[i+250][next_buffer_position] += minU2;

  }


  /* *******************************************
     Update internal variables
     ******************************************* */

  //Write output
  for (i=0; i<300; i++)
    O[i] = out_buffer[i][buffer_position];

  // NO DELAY!!!
  buffer_position = next_buffer_position;

  w1 = ns_w1;
  w2 = ns_w2;
  t1 = ns_t1;
  t2 = ns_t2;
  u1 = ns_u1;
  u2 = ns_u2;

  return 1;
}

/** Loads the data from a .mat file. */
int ArmModel::loadData(bool onlyReset) {

  char filename[20];
  sprintf(filename, "T%d.mat",inputFileNr);

  // Open MAT-File
  MATFile *mf = matOpen(filename, "r");
  if (mf == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find input file %s\n",filename);
    return -1;
  }


  if (!onlyReset) {

    /* *********************************************************
       READ ALL DATA FROM FILE
       ********************************************************* */



    // Read Xdest and store it
    mxArray *mxXdest = matGetVariable(mf, "Xdest");
    if (mxXdest == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find Xdest in input file %s\n",filename);
      return -1;
    }
    TIMESTEPS = mxGetN(mxXdest);

    csimPrintf("Read data for %d timesteps!\n", TIMESTEPS);

    Xdest = (double *) realloc(Xdest, TIMESTEPS*50*sizeof(double));
    memcpy(Xdest, mxGetPr(mxXdest), TIMESTEPS*50*sizeof(double));

    // Read Ydest and store it
    mxArray *mxYdest = matGetVariable(mf, "Ydest");
    if (mxYdest == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find Ydest in input file %s\n",filename);
      return -1;
    }
    Ydest = (double *) realloc(Ydest, TIMESTEPS*50*sizeof(double));
    memcpy(Ydest, mxGetPr(mxYdest), TIMESTEPS*50*sizeof(double));
    mxDestroyArray(mxYdest);

    // Read Theta1 and store it
    mxArray *mxTheta1 = matGetVariable(mf, "theta1");
    if (mxTheta1 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find theta1 in input file %s\n",filename);
      return -1;
    }
    theta1 = (double *) realloc(theta1, TIMESTEPS*50*sizeof(double));
    memcpy(theta1, mxGetPr(mxTheta1), TIMESTEPS*50*sizeof(double));
    mxDestroyArray(mxTheta1);

    // Read Theta2 and store it
    mxArray *mxTheta2 = matGetVariable(mf, "theta2");
    if (mxTheta2 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find theta2 in input file %s\n",filename);
      return -1;
    }
    theta2 = (double *) realloc(theta2, TIMESTEPS*50*sizeof(double));
    memcpy(theta2, mxGetPr(mxTheta2), TIMESTEPS*50*sizeof(double));
    mxDestroyArray(mxTheta2);

    // Read nu1 and store it
    mxArray *mxNu1 = matGetVariable(mf, "nu1");
    if (mxNu1 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find nu1 in input file %s\n",filename);
    return -1;
    }
    nu1 = (double *) realloc(nu1, TIMESTEPS*50*sizeof(double));
    memcpy(nu1, mxGetPr(mxNu1), TIMESTEPS*50*sizeof(double));
    mxDestroyArray(mxNu1);

    // Read nu2 and store it
    mxArray *mxNu2 = matGetVariable(mf, "nu2");
    if (mxNu2 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find nu2 in input file %s\n",filename);
      return -1;
    }
    nu2 = (double *) realloc(nu2, TIMESTEPS*50*sizeof(double));
    memcpy(nu2, mxGetPr(mxNu2), TIMESTEPS*50*sizeof(double));
    mxDestroyArray(mxNu2);


    // Read c_theta1 and store it
    mxArray *mxCTheta1 = matGetVariable(mf, "c_theta1");
    if (mxCTheta1 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find c_theta1 in input file %s\n",filename);
    return -1;
    }
    c_theta1 = (double *) realloc(c_theta1, TIMESTEPS*sizeof(double));
    memcpy(c_theta1, mxGetPr(mxCTheta1), TIMESTEPS*sizeof(double));
    mxDestroyArray(mxCTheta1);

    // Read c_theta2 and store it
    mxArray *mxCTheta2 = matGetVariable(mf, "c_theta2");
    if (mxCTheta2 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find c_theta2 in input file %s\n",filename);
      return -1;
    }
    c_theta2 = (double *) realloc(c_theta2, TIMESTEPS*sizeof(double));
    memcpy(c_theta2, mxGetPr(mxCTheta2), TIMESTEPS*sizeof(double));
    mxDestroyArray(mxCTheta2);


    // Read c_u1 and store it
    mxArray *mxCU1 = matGetVariable(mf, "c_u1");
    if (mxCU1 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find c_u1 in input file %s\n",filename);
      return -1;
    }
    c_u1 = (double *) realloc(c_u1, TIMESTEPS*sizeof(double));
    memcpy(c_u1, mxGetPr(mxCU1), TIMESTEPS*sizeof(double));
    mxDestroyArray(mxCU1);

    // Read c_u2 and store it
    mxArray *mxCU2 = matGetVariable(mf, "c_u2");
    if (mxCU2 == NULL) {
      TheCsimError.add("ArmModel::loadData Cannot find c_u2 in input file %s\n",filename);
      return -1;
    }
    c_u2 = (double *) realloc(c_u2, TIMESTEPS*sizeof(double));
    memcpy(c_u2, mxGetPr(mxCU2), TIMESTEPS*sizeof(double));
    mxDestroyArray(mxCU2);


    // Get the values which will be added as baselines.
    int pos = 0;
    for (int i=0; i<TIMESTEPS; i++) {
      for (int j=0; j<50; j++) {
	if (pos == 0) {
	  mintheta1 = theta1[0];
	  mintheta2 = theta2[0];
	  minU1 = nu1[0];
	  minU2 = nu2[0];
	  // HERE THE GET_ACT_VAL AND MAXU1, MAXU2 ARE MISSING, DO WE NEED IT?
	}
	else {
	  if (theta1[pos] < mintheta1)
	    mintheta1 = theta1[pos];
	  if (theta2[pos] < mintheta2)
	    mintheta2 = theta2[pos];
	  if (nu1[pos] < minU1)
	    minU1 = nu1[pos];
	  if (nu2[pos] < minU2)
	    minU2 = nu2[pos];
	}

	pos++;
      }
    }
    mintheta1 = fabs(mintheta1);
    mintheta2 = fabs(mintheta2);
    minU1 = fabs(minU1);
    minU2 = fabs(minU2);

  }

  // Do the rest for a simple reset


  /* *********************************************************
     INITIALIZE THE OUTPUTS OF THE MODEL
     ********************************************************* */

  for (int i=0; i<50; i++) {
    out_buffer[i][0] = Xdest[i];
    out_buffer[i+50][0] = Ydest[i];

    out_buffer[i+100][0] = theta1[i];
    if (out_buffer[i+100][0] != 0)
      out_buffer[i+100][0] += mintheta1;
    out_buffer[i+150][0] = theta2[i];
    if (out_buffer[i+150][0] != 0)
      out_buffer[i+150][0] += mintheta2;

    out_buffer[i+200][0] = nu1[i];
    if (out_buffer[i+200][0] != 0)
      out_buffer[i+200][0] += minU1;
    out_buffer[i+250][0] = nu2[i];
    if (out_buffer[i+250][0] != 0)
      out_buffer[i+250][0] += minU2;
  }

  buffer_position = 0;


  /* *********************************************************
     Calculate initial values for u, t and w
     ********************************************************* */


  // Read u1(1) and store it
  mxArray *mxU1 = matGetVariable(mf, "u1");
  if (mxU1 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find u1 in input file %s\n",filename);
    return -1;
  }
  u1 = *(mxGetPr(mxU1));
  mxDestroyArray(mxU1);

  // Read u2(1) and store it
  mxArray *mxU2 = matGetVariable(mf, "u2");
  if (mxU2 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find u2 in input file %s\n",filename);
    return -1;
  }
  u2 = *(mxGetPr(mxU2));
  mxDestroyArray(mxU2);

  // Read t1(1) and store it
  mxArray *mxT1 = matGetVariable(mf, "t1");
  if (mxT1 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find t1 in input file %s\n",filename);
    return -1;
  }
  t1 = *(mxGetPr(mxT1));
  mxDestroyArray(mxT1);

  // Read t2(1) and store it
  mxArray *mxT2 = matGetVariable(mf, "t2");
  if (mxT2 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find t2 in input file %s\n",filename);
    return -1;
  }
  t2 = *(mxGetPr(mxT2));
  mxDestroyArray(mxT2);

  // Read dt1(1) and store it
  mxArray *mxDT1 = matGetVariable(mf, "dt1");
  if (mxDT1 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find dt1 in input file %s\n",filename);
    return -1;
  }
  w1 = *(mxGetPr(mxDT1));
  mxDestroyArray(mxDT1);

  // Read dt2(1) and store it
  mxArray *mxDT2 = matGetVariable(mf, "dt2");
  if (mxDT2 == NULL) {
    TheCsimError.add("ArmModel::loadData Cannot find dt2 in input file %s\n",filename);
    return -1;
  }
  w2 = *(mxGetPr(mxDT2));
  mxDestroyArray(mxDT2);


  // Close the input file
  if (matClose(mf) == EOF) {
    TheCsimError.add("ArmModel::loadData Error while closing the input file!\n");
    return -1;
  }



  return 0;
}

/** This function is called after parameters are updated. */
int ArmModel::updateInternal() {

  nr_file_steps = (int) (model_DT / DT);

  // RELOAD THE FILE DATA, IF FILENAME HAS CHANGED !!
  if (inputFileNr != oldInputFileNr) {
    oldInputFileNr = inputFileNr;
    return loadData(false);
  }


  return 0;
}


/** Resets the information stored within the model. */
void ArmModel::reset() {

  cur_timestep = 0;
  sim_step_counter = 0;

  buffer_position = 0;

  w1 = 0;
  w2 = 0;

  u1 = 0;
  u2 = 0;

  t1 = 0;
  t2 = 0;

  // Reload and reset the data from input file
  if (inputFileNr != oldInputFileNr) {
    oldInputFileNr = inputFileNr;
    loadData(false);
  }
  else { // only reset values
    loadData(true);
  }
}




//! Distributes the scalar OP value according to the places it went.
void ArmModel::dist_OP(double IP, int output_start_index, int next_buffer_position, int time_index, char* VarName) {
  const int IPSpread = 50;
  const double sigma   = 0.8;
  const double mu      = 0;
  const double norm_factor = 1 / (sigma * sqrt(2*PI));
  const double std_dev = 2*sigma*sigma;

  int i;

  // Make the gaussian kernel.

  // WHY DO YOU MAKE IT LENGTH 50 ?????
  // YOU NEED ONLY max. 7 ELEMENTS

  double y[7];
  double max_y;
  for (i=0; i<7; i++) {
    y[i] = norm_factor * exp(-1 * (((i - 3 - mu) * (i - 3  - mu)) / std_dev));
    if ((i == 0) || (y[i] > max_y))
      max_y = y[i];
  }
  for (i=0; i<7; i++)
    y[i] /= max_y;       // Normalize in the range [0 1]

  int vi_CenterNeuron = -1;

  /*  THIS CASE NEVER OCCURS ??

    if(strcmp(ar_VarName,  "c_Xdest") ==0)
    vi_CenterNeuron = c_Xdest[time_index];
    end;
    if(strcmp(ar_VarName, "c_Ydest") ==0)
    vi_CenterNeuron = c_Ydest[time_index];
    end;
  */

  if(strcmp(VarName, "c_theta1") ==0)
    vi_CenterNeuron = (int) c_theta1[time_index];
  else if(strcmp(VarName, "c_theta2") ==0)
    vi_CenterNeuron = (int) c_theta2[time_index];
  else if(strcmp(VarName, "c_u1") ==0)
    vi_CenterNeuron = (int) c_u1[time_index];
  else if(strcmp(VarName, "c_u2") ==0)
    vi_CenterNeuron = (int) c_u2[time_index];

  // Translation of Matlab to C-Indices
  vi_CenterNeuron--;

  // Write distributed output
  for (i=0; i<50; i++)
    out_buffer[output_start_index + i][next_buffer_position] = 0;

  if (vi_CenterNeuron - 3 >= 0)
    out_buffer[output_start_index + vi_CenterNeuron - 3][next_buffer_position] = y[0] * IP;

  if (vi_CenterNeuron - 2 >= 0)
    out_buffer[output_start_index + vi_CenterNeuron - 2][next_buffer_position] = y[1] * IP;

  if (vi_CenterNeuron - 1 >= 0)
    out_buffer[output_start_index + vi_CenterNeuron - 1][next_buffer_position] = y[2] * IP;

  // The Center Neuron
  out_buffer[output_start_index + vi_CenterNeuron][next_buffer_position]      = y[3] * IP;

  if(vi_CenterNeuron + 1 < IPSpread)
    out_buffer[output_start_index + vi_CenterNeuron + 1][next_buffer_position] = y[4] * IP;

  if(vi_CenterNeuron + 2 < IPSpread)
    out_buffer[output_start_index + vi_CenterNeuron + 2][next_buffer_position] = y[5] * IP;

  if(vi_CenterNeuron + 3 < IPSpread)
    out_buffer[output_start_index + vi_CenterNeuron + 3][next_buffer_position] = y[6] * IP;

}



