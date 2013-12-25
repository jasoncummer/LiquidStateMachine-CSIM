/*! \file csim.cpp
**  \brief This is the "main" file. It contains the MEX-Interface.
*/

/*!

\mainpage CSIM Programmers Manual and Class Reference

If you are looking or information about how to use CSIM please consult
the \htmlonly <a href="../../usermanual/usermanual.html">CSIM User
Manual</a> \endhtmlonly \latexonly CSIM User Manual \endlatexonly

Here you can find information related to the C++ implementation of CSIM.

\section lists Class Documentation

- \htmlonly <a class="qindex" href="hierarchy.html">Class hierarchy</a> \endhtmlonly
- \htmlonly <a class="qindex" href="classes.html">Alphabetical Class List</a> \endhtmlonly
- \htmlonly <a class="qindex" href="annotated.html">Annoteted Class List</a> \endhtmlonly
- \htmlonly <a class="qindex" href="files.html">File List</a> \endhtmlonly
- \htmlonly <a class="qindex" href="functions.html">Class Members</a> \endhtmlonly
- \htmlonly <a class="qindex" href="globals.html">File Members</a> \endhtmlonly

\section rel Related Information

- \htmlonly <a class="qindex" href="fields.html">Setting and getting field values of objects</a> \endhtmlonly

*/

#include "csimmex.h"
#include "csimerror.h"
#include "string.h"

#include "version.i"

static int csimMexInitialized = 0;

//! Output a version string
int csimMexVersion(int nlhs, mxArray *plhs[], int nrhs, const mxArray *[])
{
  if ( !TheNetwork )
    mexErrMsgTxt("CSIM: No network initialized yet!\n");


  if ( nlhs > 0 )
    plhs[0] = mxCreateString("CSIM Version " _VERSION_STRING_ " (" __DATE__ ")");
  else
    csimPrintf("%s\n","CSIM Version " _VERSION_STRING_ " (" __DATE__ ")");

  return 0;
}

/*! The mexFunction is the gateway to and from Matlab.
*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  if ( sizeof(uint32) != 4 )
    mexErrMsgTxt("There is something wrong with uint32!!");

  if ( !csimMexInitialized ) {
    //    printf("sizeof(void *)=%i, sizeof(long)=%i, sizeof(int)=%i, sizeof(short)=%i\n",
    //   sizeof(void *),sizeof(long),sizeof(int),sizeof(short));
    /*
    ** Register an exit function which frees all memory used by CSIM
    */
    //    mexPrintf("CSIM: Initializing MEX-file.\n");
    mexAtExit(csimMexCleanUp);
    csimMexInitialized = 1;
    csimMexInit(nlhs,plhs,nrhs,prhs);
    //    return;
  }

  if ( nrhs < 1 ) {
    csimMexVersion(nlhs,plhs,nrhs,prhs);
    return;
  }

  /* get the command string */
  char *cmd;
  if ( getString(prhs[0],&cmd) )
    mexErrMsgTxt("CSIM-Usage: csim(COMMAND,...); where COMMAND must be a string.\n");

  /* depending on the command we call the corresponding csimMex<cmd> function
  ** which parses the commands given and calls the corresponding function
  ** of TheNetwork.
  **/

  TheCsimError.clear();

       if ( 0 == strcmp(cmd,"init") )       csimMexInit(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"create") )     csimMexCreate(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"set") )        csimMexSet(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"connect") )    csimMexConnect(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"get") )        csimMexGet(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"reset") )      csimMexReset(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"simulate") )   csimMexSimulate(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"export")  )    csimMexExport(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"import") )     csimMexImport(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"destroy") )    csimMexDestroy(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"list") )       csimMexList(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"ls") )         csimMexList(nlhs,plhs,nrhs,prhs);
  else if ( 0 == strcmp(cmd,"version") )    csimMexVersion(nlhs,plhs,nrhs,prhs);
  else
    { mexErrMsgTxt("CSIM-Usage: csim(COMMAND,...); Unknown command!"); }


  if ( TheCsimError.hasErrorMsg() )
    mexErrMsgTxt(TheCsimError.msg());

}
