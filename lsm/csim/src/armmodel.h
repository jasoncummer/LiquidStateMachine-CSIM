/** \file armmodel.h
**  \brief Arm model (for Prashant Joshi).
*/

#ifndef _ARMMODEL_H_
#define _ARMMODEL_H_

#include "physicalmodel.h"
#include <string>

using namespace std;

//! Test class of physical model, for an arm. WITHOUT DELAY!!
class ArmModel : public PhysicalModel {
  
  DO_REGISTERING

 public:
  /** Constructs a new arm model. */
  ArmModel();

  virtual ~ArmModel(void);

  /** Transforms the current inputs to new output values.
      \param I Array of pointers to input values from the readouts.
      \param O Array of output values.
      \return -1 if an error occured, 1 for success. */
  int transform(double** I, double* O);

  /** Resets the information stored within the model. */
  void reset();

  /** This function is called after parameters are updated. */
  int updateInternal();

  /** These parameters can be set with a csim('set',...) command. */

  //! Baseline parameter for theta1
  double mintheta1;
  //! Baseline parameter for theta2
  double mintheta2;
  //! Baseline parameter for U1
  double minU1;
  //! Baseline parameter for U2
  double minU2;


  //! The time-step of simulation.
  double model_DT;

  //! The time of simulation
  // double TSTIM = 0.5;


  //! The X coordinate of origin
  double Xo;

  //! The Y coordinate of origin
  double Yo;

  //! The mass of 1st link
  double m1;

  //! The mass of second link
  double m2;

  //! The distance of central point of link 1.
  double lc1;

  //! The distance of central point of link 2.
  double lc2;

  //! Length of first link
  double l1;

  //! Length of second link
  double l2;

  //! The moment of inertia of link1.
  double I1;

  //! The moment of inertia of link2.
  double I2;

  //! The factor by which the external noise will be  proportional to the input magnitude.
  double MFACT;

  //! The time at which the perturbation will start
  double PERT_TIME;

  //! The duration of perturbation
  double DURATION;


  //! The file-number for input of Xdest, Ydest, Theta1, Theta2 (file-name is "T(x).mat", where (x) is this number
  int inputFileNr;


 protected:
  /* ********************************************************************
     MAKE THOSE VARIABLES PROTECTED OR PUBLIC, IF YOU WANT TO RECORD THEM
     ******************************************************************** */

  //! Angle theta 1
  double t1;
  //! Angle theta 2
  double t2;

  //! Angular velocity 1 (updated every time step)
  double w1;
  //! Angular velocity 2 (updated every time step)
  double w2;

  //! Torque 1 (updated every time step)
  double u1;
  //! Torque 2 (updated every time step)
  double u2;


 private:
  /** Loads the data from a .mat file. Returns 0 for success, -1 otherwise. */
  int loadData(bool onlyReset);

  //! Distributes the scalar OP value according to the places it went.
  void dist_OP(double IP, int output_start_index, int next_buffer_position, int time_index, char* VarName);




  //! Old file-number (needed for updates) \internal [hidden;]
  int oldInputFileNr;

  //! Number of timesteps in simulation  \internal [hidden;]
  int TIMESTEPS;

  //! Current time step (in outside, not simulation time. One time step corresponds to one time-step within input files) \internal [hidden;]
  int cur_timestep;

  //! Number of simulation time-steps between file-time-steps  \internal [hidden;]
  int nr_file_steps;

  //! Counts simulation steps between file-steps  \internal [hidden;]
  int sim_step_counter;


  /* Variables read in from the input file */

  //! X-coordinate \internal [hidden;]
  double *Xdest;
  //! Y-coordinate \internal [hidden;]
  double *Ydest;
  //! Angle Theta1 \internal [hidden;]
  double *theta1;
  //! Angle Theta2 \internal [hidden;]
  double *theta2;
  //! N U1 \internal [hidden;]
  double *nu1;
  //! N U2 \internal [hidden;]
  double *nu2;
  //! Center for distribution of Theta1 \internal [hidden;]
  double *c_theta1;
  //! Center for distribution of Theta2 \internal [hidden;]
  double *c_theta2;
  //! Center for distribution of U1 \internal [hidden;]
  double *c_u1;
  //! Center for distribution of U2 \internal [hidden;]
  double *c_u2;


  //! Buffer for outputs (if you want to use delay) \internal [hidden;]
  double **out_buffer;
  //! Length of the buffer \internal [hidden;]
  int buffer_length;
  //! Position inside the buffer \internal [hidden;]
  int buffer_position;
};

#endif
