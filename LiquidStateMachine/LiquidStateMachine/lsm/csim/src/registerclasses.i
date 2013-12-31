csimClassInfo *AChannel_Hoffman97::classInfo=0;
csimClassInfo *AChannel_Korngreen02::classInfo=0;
csimClassInfo *ActiveCaChannel::classInfo=0;
csimClassInfo *ActiveChannel::classInfo=0;
csimClassInfo *AHP_Channel::classInfo=0;
csimClassInfo *AlphaSpikeFilter::classInfo=0;
csimClassInfo *AnalogFeedbackNeuron::classInfo=0;
csimClassInfo *AnalogInputNeuron::classInfo=0;
csimClassInfo *AnalogTeacher::classInfo=0;
csimClassInfo *ArmModel::classInfo=0;
csimClassInfo *bNACNeuron::classInfo=0;
csimClassInfo *bNACOUNeuron::classInfo=0;
csimClassInfo *CaChannel_Yamada98::classInfo=0;
csimClassInfo *cACNeuron::classInfo=0;
csimClassInfo *cACOUNeuron::classInfo=0;
csimClassInfo *CaGate_Yamada98::classInfo=0;
csimClassInfo *CALChannel_Destexhe98::classInfo=0;
csimClassInfo *CbHHOuINeuron::classInfo=0;
csimClassInfo *CbHHOuNeuron::classInfo=0;
csimClassInfo *CbNeuron::classInfo=0;
csimClassInfo *CbNeuronSt::classInfo=0;
csimClassInfo *CbStOuNeuron::classInfo=0;
csimClassInfo *CountSpikeFilter::classInfo=0;
csimClassInfo *DiscretizationPreprocessor::classInfo=0;
csimClassInfo *dNACNeuron::classInfo=0;
csimClassInfo *dNACOUNeuron::classInfo=0;
csimClassInfo *DynamicGlutamateSynapse::classInfo=0;
csimClassInfo *DynamicGlutamateSynapseSynchan::classInfo=0;
csimClassInfo *DynamicSpikingCbSynapse::classInfo=0;
csimClassInfo *DynamicSpikingSynapse::classInfo=0;
csimClassInfo *DynamicStdpSynapse::classInfo=0;
csimClassInfo *ExpSpikeFilter::classInfo=0;
csimClassInfo *ExtInputNeuron::classInfo=0;
csimClassInfo *ExtOutLifNeuron::classInfo=0;
csimClassInfo *ExtOutLinearNeuron::classInfo=0;
csimClassInfo *ExtOutSigmoidalNeuron::classInfo=0;
csimClassInfo *GaussianAnalogFilter::classInfo=0;
csimClassInfo *GlutamateSynapse::classInfo=0;
csimClassInfo *GlutamateSynapseSynchan::classInfo=0;
csimClassInfo *GVD_cT_Gate::classInfo=0;
csimClassInfo *GVD_Gate::classInfo=0;
csimClassInfo *HChannel_Stuart98::classInfo=0;
csimClassInfo *HH_K_Channel::classInfo=0;
csimClassInfo *HH_Na_Channel::classInfo=0;
csimClassInfo *HHNeuron::classInfo=0;
csimClassInfo *HVACAChannel_Brown93::classInfo=0;
csimClassInfo *IfbNeuron::classInfo=0;
csimClassInfo *Izhi_Neuron::classInfo=0;
csimClassInfo *KCAChannel_Mainen96::classInfo=0;
csimClassInfo *KChannel_Korngreen02::classInfo=0;
csimClassInfo *KDChannel_Traub91::classInfo=0;
csimClassInfo *LifBurstNeuron::classInfo=0;
csimClassInfo *LifNeuron::classInfo=0;
csimClassInfo *LifNeuronSynchan::classInfo=0;
csimClassInfo *linear_classification::classInfo=0;
csimClassInfo *linear_regression::classInfo=0;
csimClassInfo *LinearNeuron::classInfo=0;
csimClassInfo *LinearPreprocessor::classInfo=0;
csimClassInfo *MChannel_Mainen96::classInfo=0;
csimClassInfo *MChannel_Mainen96orig::classInfo=0;
csimClassInfo *MChannel_Wang98::classInfo=0;
csimClassInfo *Mean_Std_Preprocessor::classInfo=0;
csimClassInfo *MexRecorder::classInfo=0;
csimClassInfo *NAChannel_Traub91::classInfo=0;
csimClassInfo *NPChannel_McCormick02::classInfo=0;
csimClassInfo *PCAPreprocessor::classInfo=0;
csimClassInfo *Readout::classInfo=0;
csimClassInfo *Recorder::classInfo=0;
csimClassInfo *SICChannel_Maciokas02::classInfo=0;
csimClassInfo *SigmoidalNeuron::classInfo=0;
csimClassInfo *SpikingInputNeuron::classInfo=0;
csimClassInfo *SpikingTeacher::classInfo=0;
csimClassInfo *StaticAnalogCbSynapse::classInfo=0;
csimClassInfo *StaticAnalogSynapse::classInfo=0;
csimClassInfo *StaticGlutamateSynapse::classInfo=0;
csimClassInfo *StaticGlutamateSynapseSynchan::classInfo=0;
csimClassInfo *StaticSpikingCbSynapse::classInfo=0;
csimClassInfo *StaticSpikingSynapse::classInfo=0;
csimClassInfo *StaticStdpSynapse::classInfo=0;
csimClassInfo *Traubs_HH_K_Channel::classInfo=0;
csimClassInfo *Traubs_HH_Na_Channel::classInfo=0;
csimClassInfo *TraubsHHNeuron::classInfo=0;
csimClassInfo *TriangularAnalogFilter::classInfo=0;
csimClassInfo *UserAnalogFilter::classInfo=0;


void csimClassInfoDB::registerClasses(void)
{
  if (!AChannel_Hoffman97::classInfo ) {
    AChannel_Hoffman97::classInfo =registerCsimClass("AChannel_Hoffman97","Voltage dependent ion channel from Hoffman et al., 1997.");
    AChannel_Hoffman97*TmpObj = new AChannel_Hoffman97;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AChannel_Hoffman97::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!AChannel_Korngreen02::classInfo ) {
    AChannel_Korngreen02::classInfo =registerCsimClass("AChannel_Korngreen02","Voltage dependent ion channel from Korngreen et al. 2002.");
    AChannel_Korngreen02*TmpObj = new AChannel_Korngreen02;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AChannel_Korngreen02::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ActiveCaChannel::classInfo ) {
    ActiveCaChannel::classInfo =registerCsimClass("ActiveCaChannel","Ion channel that contributes to the intracellular calcium concentration.");
    ActiveCaChannel*TmpObj = new ActiveCaChannel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ActiveCaChannel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ActiveChannel::classInfo ) {
    ActiveChannel::classInfo =registerCsimClass("ActiveChannel","Base Class for all active ionic channels using ion gates.");
    ActiveChannel*TmpObj = new ActiveChannel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ActiveChannel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!AHP_Channel::classInfo ) {
    AHP_Channel::classInfo =registerCsimClass("AHP_Channel","AHP model with constant activation gating function and constant step increase of the calcium concentration.");
    AHP_Channel*TmpObj = new AHP_Channel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AHP_Channel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("n",n,READONLY,float,1,0,1,"","Fraction of the open conductance;");
    REGFIELD("u",u,READWRITE,float,1,0,1,"","Constant step increase in n for each spike;");
    REGFIELD("Ts",Ts,READWRITE,float,1,0,1e15,"","Time constant for the deactivation of the current;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!AlphaSpikeFilter::classInfo ) {
    AlphaSpikeFilter::classInfo =registerCsimClass("AlphaSpikeFilter","Filter which simulates spikes with alpha functions.");
    AlphaSpikeFilter*TmpObj = new AlphaSpikeFilter;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AlphaSpikeFilter::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("m_tau1",m_tau1,READWRITE,double,1,-1e30,1e30,"","Time constant 1.");
    REGFIELD("m_tau2",m_tau2,READWRITE,double,1,-1e30,1e30,"","Time constant 2.");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of Input channels to filter.");
    REGFIELD("m_dt",m_dt,READONLY,double,1,-1e30,1e30,"","Time step length.");
    REGFIELD("nValuesPerChannel",nValuesPerChannel,READONLY,int,1,1,2,"","Number of values stored for each channel in lastValues.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!AnalogFeedbackNeuron::classInfo ) {
    AnalogFeedbackNeuron::classInfo =registerCsimClass("AnalogFeedbackNeuron","An object which outputs a predefined analog signal or an analog feedback from a readout or physical model.");
    AnalogFeedbackNeuron*TmpObj = new AnalogFeedbackNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AnalogFeedbackNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("feedback",feedback,READWRITE,int,1,0,1,"","Feedback-Mode: 0 = external input, 1 = feedback");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!AnalogInputNeuron::classInfo ) {
    AnalogInputNeuron::classInfo =registerCsimClass("AnalogInputNeuron","An object which outputs a predefined analog signal.");
    AnalogInputNeuron*TmpObj = new AnalogInputNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AnalogInputNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!AnalogTeacher::classInfo ) {
    AnalogTeacher::classInfo =registerCsimClass("AnalogTeacher","Teacher for a pool of analog neurons (all teached with the same signal).");
    AnalogTeacher*TmpObj = new AnalogTeacher;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) AnalogTeacher::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ArmModel::classInfo ) {
    ArmModel::classInfo =registerCsimClass("ArmModel","Test class of physical model, for an arm. WITHOUT DELAY!!");
    ArmModel*TmpObj = new ArmModel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ArmModel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("mintheta1",mintheta1,READWRITE,double,1,-1e30,1e30,"","Baseline parameter for theta1.");
    REGFIELD("mintheta2",mintheta2,READWRITE,double,1,-1e30,1e30,"","Baseline parameter for theta2.");
    REGFIELD("minU1",minU1,READWRITE,double,1,-1e30,1e30,"","Baseline parameter for U1.");
    REGFIELD("minU2",minU2,READWRITE,double,1,-1e30,1e30,"","Baseline parameter for U2.");
    REGFIELD("model_DT",model_DT,READWRITE,double,1,-1e30,1e30,"","The time-step of simulation.");
    REGFIELD("Xo",Xo,READWRITE,double,1,-1e30,1e30,"","The X coordinate of origin.");
    REGFIELD("Yo",Yo,READWRITE,double,1,-1e30,1e30,"","The Y coordinate of origin.");
    REGFIELD("m1",m1,READWRITE,double,1,-1e30,1e30,"","The mass of 1st link.");
    REGFIELD("m2",m2,READWRITE,double,1,-1e30,1e30,"","The mass of second link.");
    REGFIELD("lc1",lc1,READWRITE,double,1,-1e30,1e30,"","The distance of central point of link 1.");
    REGFIELD("lc2",lc2,READWRITE,double,1,-1e30,1e30,"","The distance of central point of link 2.");
    REGFIELD("l1",l1,READWRITE,double,1,-1e30,1e30,"","Length of first link.");
    REGFIELD("l2",l2,READWRITE,double,1,-1e30,1e30,"","Length of second link.");
    REGFIELD("I1",I1,READWRITE,double,1,-1e30,1e30,"","The moment of inertia of link1.");
    REGFIELD("I2",I2,READWRITE,double,1,-1e30,1e30,"","The moment of inertia of link2.");
    REGFIELD("MFACT",MFACT,READWRITE,double,1,-1e30,1e30,"","The factor by which the external noise will be  proportional to the input magnitude.");
    REGFIELD("PERT_TIME",PERT_TIME,READWRITE,double,1,-1e30,1e30,"","The time at which the perturbation will start.");
    REGFIELD("DURATION",DURATION,READWRITE,double,1,-1e30,1e30,"","The duration of perturbation.");
    REGFIELD("inputFileNr",inputFileNr,READWRITE,int,1,-1e30,1e30,"","The file-number for input of Xdest, Ydest, Theta1, Theta2 (file-name is 'T(x).mat', where (x) is this number.");
    REGFIELD("t1",t1,READONLY,double,1,-1e30,1e30,"","Angle theta 1.");
    REGFIELD("t2",t2,READONLY,double,1,-1e30,1e30,"","Angle theta 2.");
    REGFIELD("w1",w1,READONLY,double,1,-1e30,1e30,"","Angular velocity 1 (updated every time step).");
    REGFIELD("w2",w2,READONLY,double,1,-1e30,1e30,"","Angular velocity 2 (updated every time step).");
    REGFIELD("u1",u1,READONLY,double,1,-1e30,1e30,"","Torque 1 (updated every time step).");
    REGFIELD("u2",u2,READONLY,double,1,-1e30,1e30,"","Torque 2 (updated every time step).");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of inputs");
    REGFIELD("nOutputChannels",nOutputChannels,READONLY,int,1,-1e30,1e30,"","Number of output channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!bNACNeuron::classInfo ) {
    bNACNeuron::classInfo =registerCsimClass("bNACNeuron","Conductance based non-accomodating spiking neuron (with spike template).");
    bNACNeuron*TmpObj = new bNACNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) bNACNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!bNACOUNeuron::classInfo ) {
    bNACOUNeuron::classInfo =registerCsimClass("bNACOUNeuron","Conductance based non-accomodating spiking neuron with Ornstein Uhlenbeck process noise.");
    bNACOUNeuron*TmpObj = new bNACOUNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) bNACOUNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("ge",ge,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("gi",gi,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("ge0",ge0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("gi0",gi0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("tau_e",tau_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("tau_i",tau_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_e",sig_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_i",sig_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("Ee",Ee,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Ei",Ei,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("OuInoise",OuInoise,READONLY,double,1,-1e30,1e30,"","noise input current");
    REGFIELD("OuGnoise",OuGnoise,READONLY,double,1,-1e30,1e30,"","noise input conductance");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CaChannel_Yamada98::classInfo ) {
    CaChannel_Yamada98::classInfo =registerCsimClass("CaChannel_Yamada98","Ca concentration dependent ion channel.");
    CaChannel_Yamada98*TmpObj = new CaChannel_Yamada98;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CaChannel_Yamada98::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("u",u,READWRITE,float,1,0,1,"Mol","Constant step increase in the calcium concentration");
    REGFIELD("Ts",Ts,READWRITE,float,1,0,1e15,"Sec","Time constant for the deactivation of the calcium concentration");
    REGFIELD("Ca",Ca,READWRITE,double,1,0,1,"Mol","Interior calcium concentration");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!cACNeuron::classInfo ) {
    cACNeuron::classInfo =registerCsimClass("cACNeuron","Conductance based accomodating spiking neuron (with spike template).");
    cACNeuron*TmpObj = new cACNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) cACNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!cACOUNeuron::classInfo ) {
    cACOUNeuron::classInfo =registerCsimClass("cACOUNeuron","Conductance based accomodating spiking neuron with Ornstein Uhlenbeck process noise.");
    cACOUNeuron*TmpObj = new cACOUNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) cACOUNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("ge",ge,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("gi",gi,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("ge0",ge0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("gi0",gi0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("tau_e",tau_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("tau_i",tau_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_e",sig_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_i",sig_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("Ee",Ee,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Ei",Ei,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("OuInoise",OuInoise,READONLY,double,1,-1e30,1e30,"","noise input current");
    REGFIELD("OuGnoise",OuGnoise,READONLY,double,1,-1e30,1e30,"","noise input conductance");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CaGate_Yamada98::classInfo ) {
    CaGate_Yamada98::classInfo =registerCsimClass("CaGate_Yamada98","Ca concentration dependent ion gate.");
    CaGate_Yamada98*TmpObj = new CaGate_Yamada98;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CaGate_Yamada98::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e15,"","Scale of the time constant $\\tau(Ca)$");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("k",k,READWRITE,int,1,1,100,"1","The exponent of the gate");
    REGFIELD("P",P,READONLY,double,1,0,1,"1","The output $P(t,V) = p(t,V)^k$ of the gate.");
    REGFIELD("p",p,READWRITE,double,1,-1e30,1e30,"","The state variable.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CALChannel_Destexhe98::classInfo ) {
    CALChannel_Destexhe98::classInfo =registerCsimClass("CALChannel_Destexhe98","Voltage dependent ion channel from Destexhe et al. (1998).");
    CALChannel_Destexhe98*TmpObj = new CALChannel_Destexhe98;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CALChannel_Destexhe98::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CbHHOuINeuron::classInfo ) {
    CbHHOuINeuron::classInfo =registerCsimClass("CbHHOuINeuron","A single compartment neuron with an arbitrary number of channels, current supplying synapses and spike template.");
    CbHHOuINeuron*TmpObj = new CbHHOuINeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CbHHOuINeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("ge",ge,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("gi",gi,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("ge0",ge0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("gi0",gi0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("tau_e",tau_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("tau_i",tau_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_e",sig_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_i",sig_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("Ee",Ee,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Ei",Ei,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("OuInoise",OuInoise,READONLY,double,1,-1e30,1e30,"","noise input current");
    REGFIELD("OuGnoise",OuGnoise,READONLY,double,1,-1e30,1e30,"","noise input conductance");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CbHHOuNeuron::classInfo ) {
    CbHHOuNeuron::classInfo =registerCsimClass("CbHHOuNeuron","A single compartment neuron with an arbitrary number of channels, current supplying synapses and spike template.");
    CbHHOuNeuron*TmpObj = new CbHHOuNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CbHHOuNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("ge",ge,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("gi",gi,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("ge0",ge0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("gi0",gi0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("tau_e",tau_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("tau_i",tau_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_e",sig_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_i",sig_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("Ee",Ee,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Ei",Ei,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("OuInoise",OuInoise,READONLY,double,1,-1e30,1e30,"","noise input current");
    REGFIELD("OuGnoise",OuGnoise,READONLY,double,1,-1e30,1e30,"","noise input conductance");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CbNeuron::classInfo ) {
    CbNeuron::classInfo =registerCsimClass("CbNeuron","A single compartment neuron with an arbitrary number of channels, conductance based, as well as current based synapses.");
    CbNeuron*TmpObj = new CbNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CbNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CbNeuronSt::classInfo ) {
    CbNeuronSt::classInfo =registerCsimClass("CbNeuronSt","A single compartment neuron with an arbitrary number of channels, coductance based as well as current based synapses and a \b spike \b template.");
    CbNeuronSt*TmpObj = new CbNeuronSt;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CbNeuronSt::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CbStOuNeuron::classInfo ) {
    CbStOuNeuron::classInfo =registerCsimClass("CbStOuNeuron","A single compartment neuron with an arbitrary number of channels, coductance based as well as current based synapses, a spike template and \b Ornstein \b Uhlenbeck \b process \b noise.");
    CbStOuNeuron*TmpObj = new CbStOuNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CbStOuNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("ge",ge,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("gi",gi,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("ge0",ge0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("gi0",gi0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("tau_e",tau_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("tau_i",tau_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_e",sig_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_i",sig_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("Ee",Ee,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Ei",Ei,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("OuInoise",OuInoise,READONLY,double,1,-1e30,1e30,"","noise input current");
    REGFIELD("OuGnoise",OuGnoise,READONLY,double,1,-1e30,1e30,"","noise input conductance");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!CountSpikeFilter::classInfo ) {
    CountSpikeFilter::classInfo =registerCsimClass("CountSpikeFilter","Filter which counts the spikes in a given time window.");
    CountSpikeFilter*TmpObj = new CountSpikeFilter;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) CountSpikeFilter::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("time_window",time_window,READWRITE,double,1,-1e30,1e30,"","Length of time window.");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of Input channels to filter.");
    REGFIELD("m_dt",m_dt,READONLY,double,1,-1e30,1e30,"","Time step length.");
    REGFIELD("nValuesPerChannel",nValuesPerChannel,READONLY,int,1,1,2,"","Number of values stored for each channel in lastValues.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!DiscretizationPreprocessor::classInfo ) {
    DiscretizationPreprocessor::classInfo =registerCsimClass("DiscretizationPreprocessor","Implementation of a discretization of the input. Every row x_i of the input vector is transformed into $x_i' = round (x_i / \epsilon_i)$.");
    DiscretizationPreprocessor*TmpObj = new DiscretizationPreprocessor;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) DiscretizationPreprocessor::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("nInputRows",nInputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for input vectors.");
    REGFIELD("nOutputRows",nOutputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for output vectors.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!dNACNeuron::classInfo ) {
    dNACNeuron::classInfo =registerCsimClass("dNACNeuron","Conductance based non-accomodating spiking neuron (with spike template).");
    dNACNeuron*TmpObj = new dNACNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) dNACNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!dNACOUNeuron::classInfo ) {
    dNACOUNeuron::classInfo =registerCsimClass("dNACOUNeuron","Conductance based non-accomodating spiking neuron with Ornstein Uhlenbeck process noise.");
    dNACOUNeuron*TmpObj = new dNACOUNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) dNACOUNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("ge",ge,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("gi",gi,READWRITE,double,1,-1e30,1e30,"S","exc and inh conductances (noise)");
    REGFIELD("ge0",ge0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("gi0",gi0,READWRITE,double,1,-1e30,1e30,"S","exc and inh mean conductances (noise)");
    REGFIELD("tau_e",tau_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("tau_i",tau_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_e",sig_e,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("sig_i",sig_i,READWRITE,double,1,-1e30,1e30,"S","time constants and std for exc and inh conductances (noise)");
    REGFIELD("Ee",Ee,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("Ei",Ei,READWRITE,double,1,-1e30,1e30,"V","Reversal potential for exc and inh currents (noise)");
    REGFIELD("STempHeight",STempHeight,READWRITE,double,1,0,1,"Volt","Height");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("OuInoise",OuInoise,READONLY,double,1,-1e30,1e30,"","noise input current");
    REGFIELD("OuGnoise",OuGnoise,READONLY,double,1,-1e30,1e30,"","noise input conductance");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!DynamicGlutamateSynapse::classInfo ) {
    DynamicGlutamateSynapse::classInfo =registerCsimClass("DynamicGlutamateSynapse","Base class for dynamic spiking glutamate synapses with spike time dependent plasticity (STDP).");
    DynamicGlutamateSynapse*TmpObj = new DynamicGlutamateSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) DynamicGlutamateSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("U",U,READWRITE,float,1,1e-5,1,"","The use parameter of the dynamic synapse");
    REGFIELD("D",D,READWRITE,float,1,0,10,"sec","The time constant of the depression of the dynamic synapse");
    REGFIELD("F",F,READWRITE,float,1,0,10,"sec","The time constant of the facilitation of the dynamic synapse");
    REGFIELD("u0",u0,READWRITE,float,1,0,1,"","Value of the time varying facilitation state variable $u$  for the first spike");
    REGFIELD("r0",r0,READWRITE,float,1,0,1,"","Value of the time varying depression state variable $r$ for the first spike");
    REGFIELD("u",u,READONLY,double,1,0,1,"","The time varying state variable $u$ for facilitation");
    REGFIELD("r",r,READONLY,double,1,0,1,"","The time varying state variable $u$ for depression");
    REGFIELD("tau_nmda",tau_nmda,READWRITE,float,1,0,100,"sec","The NMDA time constant $\\tau_{nmda}$");
    REGFIELD("Mg_conc",Mg_conc,READWRITE,float,1,-1e30,1e30,"mMol","Mg-concentration for voltage-dependence of NMDA-channel in");
    REGFIELD("E_nmda",E_nmda,READWRITE,float,1,-1e30,1e30,"V","Reversal Potential for NMDA-Receptors");
    REGFIELD("E_ampa",E_ampa,READWRITE,float,1,-1e30,1e30,"V","Reversal Potential for AMPA-Receptors");
    REGFIELD("fact_nmda",fact_nmda,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.");
    REGFIELD("fact_ampa",fact_ampa,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.");
    REGFIELD("psr_nmda",psr_nmda,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) for nmda channels.");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!DynamicGlutamateSynapseSynchan::classInfo ) {
    DynamicGlutamateSynapseSynchan::classInfo =registerCsimClass("DynamicGlutamateSynapseSynchan","Base class for all dynamic spiking synapses with spike time dependent plasticity (STDP).");
    DynamicGlutamateSynapseSynchan*TmpObj = new DynamicGlutamateSynapseSynchan;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) DynamicGlutamateSynapseSynchan::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("U",U,READWRITE,float,1,1e-5,1,"","The use parameter of the dynamic synapse");
    REGFIELD("D",D,READWRITE,float,1,0,10,"sec","The time constant of the depression of the dynamic synapse");
    REGFIELD("F",F,READWRITE,float,1,0,10,"sec","The time constant of the facilitation of the dynamic synapse");
    REGFIELD("u0",u0,READWRITE,float,1,0,1,"","Value of the time varying facilitation state variable $u$  for the first spike");
    REGFIELD("r0",r0,READWRITE,float,1,0,1,"","Value of the time varying depression state variable $r$ for the first spike");
    REGFIELD("u",u,READONLY,double,1,0,1,"","The time varying state variable $u$ for facilitation");
    REGFIELD("r",r,READONLY,double,1,0,1,"","The time varying state variable $u$ for depression");
    REGFIELD("fact_nmda",fact_nmda,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.");
    REGFIELD("fact_ampa",fact_ampa,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!DynamicSpikingCbSynapse::classInfo ) {
    DynamicSpikingCbSynapse::classInfo =registerCsimClass("DynamicSpikingCbSynapse","A dynamic spiking synapse (see Markram et al, 1998).");
    DynamicSpikingCbSynapse*TmpObj = new DynamicSpikingCbSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) DynamicSpikingCbSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("E",E,READWRITE,double,1,-1e30,1e30,"","Equilibrium potential given by the Nernst equation.");
    REGFIELD("U",U,READWRITE,float,1,1e-5,1,"","The use parameter of the dynamic synapse");
    REGFIELD("D",D,READWRITE,float,1,0,10,"sec","The time constant of the depression of the dynamic synapse");
    REGFIELD("F",F,READWRITE,float,1,0,10,"sec","The time constant of the facilitation of the dynamic synapse");
    REGFIELD("u0",u0,READWRITE,float,1,0,1,"","Value of the time varying facilitation state variable $u$  for the first spike");
    REGFIELD("r0",r0,READWRITE,float,1,0,1,"","Value of the time varying depression state variable $r$ for the first spike");
    REGFIELD("u",u,READONLY,double,1,0,1,"","The time varying state variable $u$ for facilitation");
    REGFIELD("r",r,READONLY,double,1,0,1,"","The time varying state variable $u$ for depression");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!DynamicSpikingSynapse::classInfo ) {
    DynamicSpikingSynapse::classInfo =registerCsimClass("DynamicSpikingSynapse","A dynamic spiking synapse (see Markram et al, 1998).");
    DynamicSpikingSynapse*TmpObj = new DynamicSpikingSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) DynamicSpikingSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("U",U,READWRITE,float,1,1e-5,1,"","The use parameter of the dynamic synapse");
    REGFIELD("D",D,READWRITE,float,1,0,10,"sec","The time constant of the depression of the dynamic synapse");
    REGFIELD("F",F,READWRITE,float,1,0,10,"sec","The time constant of the facilitation of the dynamic synapse");
    REGFIELD("u0",u0,READWRITE,float,1,0,1,"","Value of the time varying facilitation state variable $u$  for the first spike");
    REGFIELD("r0",r0,READWRITE,float,1,0,1,"","Value of the time varying depression state variable $r$ for the first spike");
    REGFIELD("u",u,READONLY,double,1,0,1,"","The time varying state variable $u$ for facilitation");
    REGFIELD("r",r,READONLY,double,1,0,1,"","The time varying state variable $u$ for depression");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!DynamicStdpSynapse::classInfo ) {
    DynamicStdpSynapse::classInfo =registerCsimClass("DynamicStdpSynapse","Synapse with spike time dependent plasticity as well as short term dynamics (faciliataion and depression).");
    DynamicStdpSynapse*TmpObj = new DynamicStdpSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) DynamicStdpSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("U",U,READWRITE,float,1,1e-5,1,"","The use parameter of the dynamic synapse");
    REGFIELD("D",D,READWRITE,float,1,0,10,"sec","The time constant of the depression of the dynamic synapse");
    REGFIELD("F",F,READWRITE,float,1,0,10,"sec","The time constant of the facilitation of the dynamic synapse");
    REGFIELD("u0",u0,READWRITE,float,1,0,1,"","Value of the time varying facilitation state variable $u$  for the first spike");
    REGFIELD("r0",r0,READWRITE,float,1,0,1,"","Value of the time varying depression state variable $r$ for the first spike");
    REGFIELD("u",u,READONLY,double,1,0,1,"","The time varying state variable $u$ for facilitation");
    REGFIELD("r",r,READONLY,double,1,0,1,"","The time varying state variable $u$ for depression");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ExpSpikeFilter::classInfo ) {
    ExpSpikeFilter::classInfo =registerCsimClass("ExpSpikeFilter","Filter which simulates exponential decay of spikes.");
    ExpSpikeFilter*TmpObj = new ExpSpikeFilter;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ExpSpikeFilter::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("m_tau1",m_tau1,READWRITE,double,1,-1e30,1e30,"","Time constant.");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of Input channels to filter.");
    REGFIELD("m_dt",m_dt,READONLY,double,1,-1e30,1e30,"","Time step length.");
    REGFIELD("nValuesPerChannel",nValuesPerChannel,READONLY,int,1,1,2,"","Number of values stored for each channel in lastValues.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ExtInputNeuron::classInfo ) {
    ExtInputNeuron::classInfo =registerCsimClass("ExtInputNeuron","Implements an external (analog) input neuron.");
    ExtInputNeuron*TmpObj = new ExtInputNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ExtInputNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("myIndex",myIndex,READWRITE,int,1,-1e30,1e30,"","ID of external input (is equal index of array)");
    REGFIELD("beReal",beReal,READWRITE,int,1,-1e30,1e30,"","Flag if external input is used or normal input");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ExtOutLifNeuron::classInfo ) {
    ExtOutLifNeuron::classInfo =registerCsimClass("ExtOutLifNeuron","A LIF neuron which writes its output to some external program.");
    ExtOutLifNeuron*TmpObj = new ExtOutLifNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ExtOutLifNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("myIndex",myIndex,READWRITE,int,1,-1e30,1e30,"","ID of external input (is equal index of array)");
    REGFIELD("beReal",beReal,READWRITE,int,1,-1e30,1e30,"","Flag if external input is used or normal input");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","The initial condition for $V_m$ at time $t=0$.");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","The length of the absolute refractory period.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"A","The standard deviation of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","A constant current to be injected into the LIF neuron.");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Isyn",Isyn,READONLY,float,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage $V_m$");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ExtOutLinearNeuron::classInfo ) {
    ExtOutLinearNeuron::classInfo =registerCsimClass("ExtOutLinearNeuron","A linear neuron which writes its output to some external program.");
    ExtOutLinearNeuron*TmpObj = new ExtOutLinearNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ExtOutLinearNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Iinject",Iinject,READWRITE,double,1,-1e30,1e30,"W","external current injected into neuron");
    REGFIELD("Vinit",Vinit,READWRITE,double,1,-1e30,1e30,"V","initial 'membrane voltage'");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("myIndex",myIndex,READWRITE,int,1,-1e30,1e30,"","ID of external input (is equal index of array)");
    REGFIELD("beReal",beReal,READWRITE,int,1,-1e30,1e30,"","Flag if external input is used or normal input");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!ExtOutSigmoidalNeuron::classInfo ) {
    ExtOutSigmoidalNeuron::classInfo =registerCsimClass("ExtOutSigmoidalNeuron","A sigmoidal neuron which writes its output to some external program.");
    ExtOutSigmoidalNeuron*TmpObj = new ExtOutSigmoidalNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) ExtOutSigmoidalNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("thresh",thresh,READWRITE,double,1,-1e30,1e30,"W","Itot = logsig(beta*(Itot-thresh))");
    REGFIELD("beta",beta,READWRITE,double,1,-1e30,1e30,"1","Itot = logsig(beta*(Itot-thresh))");
    REGFIELD("tau_m",tau_m,READWRITE,double,1,-1e30,1e30,"","time constant");
    REGFIELD("A_max",A_max,READWRITE,double,1,-1e30,1e30,"1","the output of a sig neuron is scaled to (0,W_max)");
    REGFIELD("I_inject",I_inject,READWRITE,double,1,-1e30,1e30,"W","external current injected into neuron");
    REGFIELD("Vm_init",Vm_init,READWRITE,double,1,-1e30,1e30,"V","initial 'membrane voltage'");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("myIndex",myIndex,READWRITE,int,1,-1e30,1e30,"","ID of external input (is equal index of array)");
    REGFIELD("beReal",beReal,READWRITE,int,1,-1e30,1e30,"","Flag if external input is used or normal input");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!GaussianAnalogFilter::classInfo ) {
    GaussianAnalogFilter::classInfo =registerCsimClass("GaussianAnalogFilter","Implementation of an analog low-pass filter with a gaussian kernel.");
    GaussianAnalogFilter*TmpObj = new GaussianAnalogFilter;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) GaussianAnalogFilter::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("m_std_dev",m_std_dev,READWRITE,double,1,-1e30,1e30,"","Standard deviation of the filter mask.");
    REGFIELD("nKernelLength",nKernelLength,READWRITE,int,1,-1e30,1e30,"","The length of the kernel.");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of Input channels to filter.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!GlutamateSynapse::classInfo ) {
    GlutamateSynapse::classInfo =registerCsimClass("GlutamateSynapse","Base Class for Glutamate synapse with NMDA and AMPA channels.");
    GlutamateSynapse*TmpObj = new GlutamateSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) GlutamateSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("tau_nmda",tau_nmda,READWRITE,float,1,0,100,"sec","The NMDA time constant $\\tau_{nmda}$");
    REGFIELD("Mg_conc",Mg_conc,READWRITE,float,1,-1e30,1e30,"mMol","Mg-concentration for voltage-dependence of NMDA-channel in");
    REGFIELD("E_nmda",E_nmda,READWRITE,float,1,-1e30,1e30,"V","Reversal Potential for NMDA-Receptors");
    REGFIELD("E_ampa",E_ampa,READWRITE,float,1,-1e30,1e30,"V","Reversal Potential for AMPA-Receptors");
    REGFIELD("fact_nmda",fact_nmda,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.");
    REGFIELD("fact_ampa",fact_ampa,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.");
    REGFIELD("psr_nmda",psr_nmda,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) for nmda channels.");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!GlutamateSynapseSynchan::classInfo ) {
    GlutamateSynapseSynchan::classInfo =registerCsimClass("GlutamateSynapseSynchan","Base Class for Glutamate synapse with NMDA and AMPA channels at the target neuron.");
    GlutamateSynapseSynchan*TmpObj = new GlutamateSynapseSynchan;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) GlutamateSynapseSynchan::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("fact_nmda",fact_nmda,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.");
    REGFIELD("fact_ampa",fact_ampa,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!GVD_cT_Gate::classInfo ) {
    GVD_cT_Gate::classInfo =registerCsimClass("GVD_cT_Gate","A generic voltage dependent ion gate with constant time constant.");
    GVD_cT_Gate*TmpObj = new GVD_cT_Gate;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) GVD_cT_Gate::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vh",Vh,READWRITE,double,1,-1,1,"Volt","Inflection point of the sigmoidal function $p(V)$");
    REGFIELD("Vc",Vc,READWRITE,double,1,-1,1,"Volt","Slope of the sigmoidal function $p(V)$");
    REGFIELD("Ts",Ts,READWRITE,double,1,-1,1,"","Voltage independent time constant $\\tau$");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("k",k,READWRITE,int,1,1,100,"1","The exponent of the gate");
    REGFIELD("P",P,READONLY,double,1,0,1,"1","The output $P(t,V) = p(t,V)^k$ of the gate.");
    REGFIELD("p",p,READWRITE,double,1,-1e30,1e30,"","The state variable.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!GVD_Gate::classInfo ) {
    GVD_Gate::classInfo =registerCsimClass("GVD_Gate","A generic voltage dependent ion gate.");
    GVD_Gate*TmpObj = new GVD_Gate;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) GVD_Gate::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vh",Vh,READWRITE,double,1,-1,1,"Volt","Inflection point of the sigmoidal function $p(V)$");
    REGFIELD("Vc",Vc,READWRITE,double,1,-1,1,"Volt","Determines the slope of the sigmoidal function $p(V)$");
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e15,"","Scale of the time constant $\\tau(V)$");
    REGFIELD("Te",Te,READWRITE,double,1,-1,1,"","Additional exponential factor of the time constant $\\tau(V)$");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("k",k,READWRITE,int,1,1,100,"1","The exponent of the gate");
    REGFIELD("P",P,READONLY,double,1,0,1,"1","The output $P(t,V) = p(t,V)^k$ of the gate.");
    REGFIELD("p",p,READWRITE,double,1,-1e30,1e30,"","The state variable.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!HChannel_Stuart98::classInfo ) {
    HChannel_Stuart98::classInfo =registerCsimClass("HChannel_Stuart98","Voltage dependent ion channel from Stuart and Spruston (1998).");
    HChannel_Stuart98*TmpObj = new HChannel_Stuart98;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) HChannel_Stuart98::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!HH_K_Channel::classInfo ) {
    HH_K_Channel::classInfo =registerCsimClass("HH_K_Channel","Hodgkin and Huxley fast potassium (K) channel for AP generation.");
    HH_K_Channel*TmpObj = new HH_K_Channel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) HH_K_Channel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!HH_Na_Channel::classInfo ) {
    HH_Na_Channel::classInfo =registerCsimClass("HH_Na_Channel","Hodgkin and Huxley fast sodium (Na) channel for AP generation.");
    HH_Na_Channel*TmpObj = new HH_Na_Channel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) HH_Na_Channel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!HHNeuron::classInfo ) {
    HHNeuron::classInfo =registerCsimClass("HHNeuron","Conductance based spiking neuron using the HH squid modell.");
    HHNeuron*TmpObj = new HHNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) HHNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!HVACAChannel_Brown93::classInfo ) {
    HVACAChannel_Brown93::classInfo =registerCsimClass("HVACAChannel_Brown93","Voltage dependent ion channel from Brown et al. (1993).");
    HVACAChannel_Brown93*TmpObj = new HVACAChannel_Brown93;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) HVACAChannel_Brown93::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!IfbNeuron::classInfo ) {
    IfbNeuron::classInfo =registerCsimClass("IfbNeuron","A leaky-integrate-and-fire-or-burst (IFB) neuron.");
    IfbNeuron*TmpObj = new IfbNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) IfbNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","The initial condition for $V_m$ at time $t=0$.");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","The length of the absolute refractory period.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"A","The standard deviation of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","A constant current to be injected into the LIF neuron.");
    REGFIELD("h",h,READWRITE,float,1,-1e30,1e30,"","internal parameter h - deinactivation of Ca-channels");
    REGFIELD("tau_p",tau_p,READWRITE,float,1,-1e30,1e30,"","tau_p time constant for recovery from inactivation");
    REGFIELD("tau_m",tau_m,READWRITE,float,1,-1e30,1e30,"","tau_m time constant for inactivation");
    REGFIELD("Vh",Vh,READWRITE,float,1,-1e30,1e30,"","Vh constant for onset of Ca-channel activation.");
    REGFIELD("gh",gh,READWRITE,float,1,-1e30,1e30,"","gh constant for relative impact of Ca-current to leak current (0.5-2)");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage $V_m$");
    REGFIELD("Isyn",Isyn,READONLY,float,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!Izhi_Neuron::classInfo ) {
    Izhi_Neuron::classInfo =registerCsimClass("Izhi_Neuron","A canonical bursting and spiking neuron.");
    Izhi_Neuron*TmpObj = new Izhi_Neuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) Izhi_Neuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","The initial condition for $V_m$ at time $t=0$.");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","The length of the absolute refractory period.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"A","The standard deviation of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","A constant current to be injected into the LIF neuron.");
    REGFIELD("a",a,READWRITE,float,1,-1e30,1e30,"","A constant (0.02, 01) describing the coupling of variable u to Vm;.");
    REGFIELD("b",b,READWRITE,float,1,-1e30,1e30,"","A constant controlling sensitivity og u.");
    REGFIELD("c",c,READWRITE,float,1,-1e30,1e30,"","A constant controlling reset of Vm (1000*Vreset).");
    REGFIELD("d",d,READWRITE,float,1,-1e30,1e30,"","A constant controlling reset of u.");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage $V_m$");
    REGFIELD("Vb",Vb,READONLY,double,1,-1e30,1e30,"","The membrane voltage in millivolt.");
    REGFIELD("Vint",Vint,READONLY,double,1,-1e30,1e30,"","The membrane voltage in millivolt.");
    REGFIELD("u",u,READONLY,double,1,-1e30,1e30,"","internal variable");
    REGFIELD("ub",ub,READONLY,double,1,-1e30,1e30,"","The internal variable u adapted to millivolt and millisecond.");
    REGFIELD("Isyn",Isyn,READONLY,float,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!KCAChannel_Mainen96::classInfo ) {
    KCAChannel_Mainen96::classInfo =registerCsimClass("KCAChannel_Mainen96","Voltage dependent ion gate from Mainen and Sejnowski (1996).");
    KCAChannel_Mainen96*TmpObj = new KCAChannel_Mainen96;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) KCAChannel_Mainen96::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!KChannel_Korngreen02::classInfo ) {
    KChannel_Korngreen02::classInfo =registerCsimClass("KChannel_Korngreen02","Voltage dependent ion channel from Korngreen et al. 2002.");
    KChannel_Korngreen02*TmpObj = new KChannel_Korngreen02;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) KChannel_Korngreen02::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!KDChannel_Traub91::classInfo ) {
    KDChannel_Traub91::classInfo =registerCsimClass("KDChannel_Traub91","");
    KDChannel_Traub91*TmpObj = new KDChannel_Traub91;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) KDChannel_Traub91::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!LifBurstNeuron::classInfo ) {
    LifBurstNeuron::classInfo =registerCsimClass("LifBurstNeuron","A nonstandart leaky-integrate-and-fire (I&F) neuron.");
    LifBurstNeuron*TmpObj = new LifBurstNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) LifBurstNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The default voltage to reset $V_m$ to after a spike.");
    REGFIELD("VBthresh",VBthresh,READWRITE,float,1,-1,1,"V","spike pattern dependent treshold $V_m$ to after a spike.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","The initial condition for $V_m$ at time $t=0$.");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","The length of the absolute refractory period.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"A","The standard deviation of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","A constant current to be injected into the LIF neuron.");
    REGFIELD("uB",uB,READWRITE,float,1,-1e30,1e30,"","internal parameter uB - recovery from spike facilitation with time constant FB");
    REGFIELD("rB",rB,READWRITE,float,1,-1e30,1e30,"","internal parameter rB - recovery from spike depression with time constant DB");
    REGFIELD("FB",FB,READWRITE,float,1,-1e30,1e30,"","FB time constant for recovery from facilitation of spike generation.");
    REGFIELD("DB",DB,READWRITE,float,1,-1e30,1e30,"","DB time constant for recovery from depression of spike generation.");
    REGFIELD("UB",UB,READWRITE,float,1,-1e30,1e30,"","UB constant for onset of facilitation of spike generation.");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage $V_m$");
    REGFIELD("Isyn",Isyn,READONLY,float,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!LifNeuron::classInfo ) {
    LifNeuron::classInfo =registerCsimClass("LifNeuron","A leaky-integrate-and-fire (I&F) neuron.");
    LifNeuron*TmpObj = new LifNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) LifNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","The initial condition for $V_m$ at time $t=0$.");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","The length of the absolute refractory period.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"A","The standard deviation of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","A constant current to be injected into the LIF neuron.");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Isyn",Isyn,READONLY,float,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage $V_m$");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!LifNeuronSynchan::classInfo ) {
    LifNeuronSynchan::classInfo =registerCsimClass("LifNeuronSynchan","A leaky-integrate-and-fire (I&F) neuron.");
    LifNeuronSynchan*TmpObj = new LifNeuronSynchan;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) LifNeuronSynchan::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("tau_nmda",tau_nmda,READWRITE,float,1,0,1000,"sec","The time constant for NMDA channels.");
    REGFIELD("tau_ampa",tau_ampa,READWRITE,float,1,0,1000,"sec","The time constant for AMPA channels.");
    REGFIELD("tau_gaba_a",tau_gaba_a,READWRITE,float,1,0,1000,"sec","The time constant for GABA_A channels.");
    REGFIELD("tau_gaba_b",tau_gaba_b,READWRITE,float,1,0,1000,"sec","The time constant for GABA_B channels.");
    REGFIELD("E_nmda",E_nmda,READWRITE,float,1,-1e30,1e30,"V","The reversal potential for NMDA channels.");
    REGFIELD("E_ampa",E_ampa,READWRITE,float,1,-1e30,1e30,"V","The reversal potential for AMPA channels.");
    REGFIELD("E_gaba_a",E_gaba_a,READWRITE,float,1,-1e30,1e30,"V","The reversal potential for GABA_A channels.");
    REGFIELD("E_gaba_b",E_gaba_b,READWRITE,float,1,-1e30,1e30,"V","The reversal potential for GABA_B channels.");
    REGFIELD("Mg_conc",Mg_conc,READWRITE,float,1,-1e30,1e30,"mMol","Mg-concentration for voltage-dependence of NMDA-channel in");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","The initial condition for $V_m$ at time $t=0$.");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","The length of the absolute refractory period.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"A","The standard deviation of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","A constant current to be injected into the LIF neuron.");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("summationPoint_nmda",summationPoint_nmda,READONLY,double,1,-1e30,1e30,"","At this point synaptic input from NMDA-channels is summed up.");
    REGFIELD("summationPoint_ampa",summationPoint_ampa,READONLY,double,1,-1e30,1e30,"","At this point synaptic input from AMPA-channels is summed up.");
    REGFIELD("summationPoint_gaba_a",summationPoint_gaba_a,READONLY,double,1,-1e30,1e30,"","At this point synaptic input from GABA_A-channels is summed up.");
    REGFIELD("summationPoint_gaba_b",summationPoint_gaba_b,READONLY,double,1,-1e30,1e30,"","At this point synaptic input from GABA_B-channels is summed up.");
    REGFIELD("Isyn",Isyn,READONLY,float,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage $V_m$");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!linear_classification::classInfo ) {
    linear_classification::classInfo =registerCsimClass("linear_classification","Implementation of a linear classification.");
    linear_classification*TmpObj = new linear_classification;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) linear_classification::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("nClasses",nClasses,READWRITE,int,1,-1e30,1e30,"","Number of Classes.");
    REGFIELD("range_low",range_low,READWRITE,double,1,-1e30,1e30,"","Lower bound of algorithms range.");
    REGFIELD("range_high",range_high,READWRITE,double,1,-1e30,1e30,"","Upper bound of algorithms range.");
    REGFIELD("nInputRows",nInputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for input vectors.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!linear_regression::classInfo ) {
    linear_regression::classInfo =registerCsimClass("linear_regression","Implementation of a linear regression.");
    linear_regression*TmpObj = new linear_regression;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) linear_regression::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("range_low",range_low,READWRITE,double,1,-1e30,1e30,"","Lower bound of algorithms range.");
    REGFIELD("range_high",range_high,READWRITE,double,1,-1e30,1e30,"","Upper bound of algorithms range.");
    REGFIELD("nInputRows",nInputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for input vectors.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!LinearNeuron::classInfo ) {
    LinearNeuron::classInfo =registerCsimClass("LinearNeuron","A linear neuron: simply summing up the inputs.");
    LinearNeuron*TmpObj = new LinearNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) LinearNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Iinject",Iinject,READWRITE,double,1,-1e30,1e30,"W","external current injected into neuron");
    REGFIELD("Vinit",Vinit,READWRITE,double,1,-1e30,1e30,"V","initial 'membrane voltage'");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!LinearPreprocessor::classInfo ) {
    LinearPreprocessor::classInfo =registerCsimClass("LinearPreprocessor","Implementation of a linear transformation of the input. Every row x_i of the input vector is transformed into x_i' = a_i * x_i + b_i.");
    LinearPreprocessor*TmpObj = new LinearPreprocessor;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) LinearPreprocessor::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("nInputRows",nInputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for input vectors.");
    REGFIELD("nOutputRows",nOutputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for output vectors.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!MChannel_Mainen96::classInfo ) {
    MChannel_Mainen96::classInfo =registerCsimClass("MChannel_Mainen96","Voltage dependent ion channel from Mainen and Sejnowski (1996).");
    MChannel_Mainen96*TmpObj = new MChannel_Mainen96;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) MChannel_Mainen96::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!MChannel_Mainen96orig::classInfo ) {
    MChannel_Mainen96orig::classInfo =registerCsimClass("MChannel_Mainen96orig","");
    MChannel_Mainen96orig*TmpObj = new MChannel_Mainen96orig;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) MChannel_Mainen96orig::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!MChannel_Wang98::classInfo ) {
    MChannel_Wang98::classInfo =registerCsimClass("MChannel_Wang98","Voltage dependent ion channel from Wang et al., 1998.");
    MChannel_Wang98*TmpObj = new MChannel_Wang98;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) MChannel_Wang98::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!Mean_Std_Preprocessor::classInfo ) {
    Mean_Std_Preprocessor::classInfo =registerCsimClass("Mean_Std_Preprocessor","Implementation of a Mean / Standard-Deviation Normalizer.");
    Mean_Std_Preprocessor*TmpObj = new Mean_Std_Preprocessor;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) Mean_Std_Preprocessor::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("nInputRows",nInputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for input vectors.");
    REGFIELD("nOutputRows",nOutputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for output vectors.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!MexRecorder::classInfo ) {
    MexRecorder::classInfo =registerCsimClass("MexRecorder","Alias for Recorder for backwards compatibility.");
    MexRecorder*TmpObj = new MexRecorder;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) MexRecorder::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("commonChannels",commonChannels,READWRITE,int,1,0,1,"","Flag: 1 ... output all channels in one matrix (WARNING: no spikes are returned yet), 0 ... output each recorded field as seperate channel");
    REGFIELD("dt",dt,READWRITE,double,1,0,100,"sec","The timestep at which an recording should be done (no meaning if recording spikes).");
    REGFIELD("Tprealloc",Tprealloc,READWRITE,double,1,0,100,"sec","Provide your best guess how long the network will be simulated (in simulation time).");
    REGFIELD("enabled",enabled,READWRITE,int,1,0,1,"","Flag: 0 ... recorder disabled, 1 ... recoder enabled");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!NAChannel_Traub91::classInfo ) {
    NAChannel_Traub91::classInfo =registerCsimClass("NAChannel_Traub91","");
    NAChannel_Traub91*TmpObj = new NAChannel_Traub91;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) NAChannel_Traub91::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!NPChannel_McCormick02::classInfo ) {
    NPChannel_McCormick02::classInfo =registerCsimClass("NPChannel_McCormick02","Voltage dependent ion channel from McCormick and Huguenard (1992).");
    NPChannel_McCormick02*TmpObj = new NPChannel_McCormick02;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) NPChannel_McCormick02::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Ts",Ts,READWRITE,double,1,0,1e2,"","Scale of the time constants of the gating variables $\\tau(V)$");
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!PCAPreprocessor::classInfo ) {
    PCAPreprocessor::classInfo =registerCsimClass("PCAPreprocessor","Implementation of a PCA (Principal Component Analysis) of the input. A principal component transformation is applied to the whole input vector.");
    PCAPreprocessor*TmpObj = new PCAPreprocessor;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) PCAPreprocessor::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("nInputRows",nInputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for input vectors.");
    REGFIELD("nOutputRows",nOutputRows,READONLY,int,1,-1e30,1e30,"","Number of rows for output vectors.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!Readout::classInfo ) {
    Readout::classInfo =registerCsimClass("Readout","Base class of all readouts.");
    Readout*TmpObj = new Readout;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) Readout::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("enabled",enabled,READWRITE,int,1,0,1,"","Flag: 0 ... readout disabled, 1 ... readout enabled");
    REGFIELD("offset",offset,READWRITE,double,1,-1e30,1e30,"","Offset of output values.");
    REGFIELD("range",range,READWRITE,double,1,-1e30,1e30,"","Range of output values.");
    REGFIELD("output",output,READONLY,double,1,-1e30,1e30,"","Output variable of a readout.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!Recorder::classInfo ) {
    Recorder::classInfo =registerCsimClass("Recorder","Records fields from arbitrary objects during simulation.");
    Recorder*TmpObj = new Recorder;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) Recorder::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("commonChannels",commonChannels,READWRITE,int,1,0,1,"","Flag: 1 ... output all channels in one matrix (WARNING: no spikes are returned yet), 0 ... output each recorded field as seperate channel");
    REGFIELD("dt",dt,READWRITE,double,1,0,100,"sec","The timestep at which an recording should be done (no meaning if recording spikes).");
    REGFIELD("Tprealloc",Tprealloc,READWRITE,double,1,0,100,"sec","Provide your best guess how long the network will be simulated (in simulation time).");
    REGFIELD("enabled",enabled,READWRITE,int,1,0,1,"","Flag: 0 ... recorder disabled, 1 ... recoder enabled");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!SICChannel_Maciokas02::classInfo ) {
    SICChannel_Maciokas02::classInfo =registerCsimClass("SICChannel_Maciokas02","Voltage dependent ion channel from Maciokas et al. 2002.");
    SICChannel_Maciokas02*TmpObj = new SICChannel_Maciokas02;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) SICChannel_Maciokas02::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!SigmoidalNeuron::classInfo ) {
    SigmoidalNeuron::classInfo =registerCsimClass("SigmoidalNeuron","An analog neuron with a sigmoidal activation function.");
    SigmoidalNeuron*TmpObj = new SigmoidalNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) SigmoidalNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("thresh",thresh,READWRITE,double,1,-1e30,1e30,"W","Itot = logsig(beta*(Itot-thresh))");
    REGFIELD("beta",beta,READWRITE,double,1,-1e30,1e30,"1","Itot = logsig(beta*(Itot-thresh))");
    REGFIELD("tau_m",tau_m,READWRITE,double,1,-1e30,1e30,"","time constant");
    REGFIELD("A_max",A_max,READWRITE,double,1,-1e30,1e30,"1","the output of a sig neuron is scaled to (0,W_max)");
    REGFIELD("I_inject",I_inject,READWRITE,double,1,-1e30,1e30,"W","external current injected into neuron");
    REGFIELD("Vm_init",Vm_init,READWRITE,double,1,-1e30,1e30,"V","initial 'membrane voltage'");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The current output (potential) of this neuron");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Inoise",Inoise,READWRITE,double,1,-1e30,1e30,"A^2","The noise of analog neurons");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("VmOut",VmOut,READONLY,double,1,-1e30,1e30,"","The vlaue wich will actualle be propagated to the outgoing synapses.");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!SpikingInputNeuron::classInfo ) {
    SpikingInputNeuron::classInfo =registerCsimClass("SpikingInputNeuron","A spiking neuron which emits a predefined spike train.");
    SpikingInputNeuron*TmpObj = new SpikingInputNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) SpikingInputNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!SpikingTeacher::classInfo ) {
    SpikingTeacher::classInfo =registerCsimClass("SpikingTeacher","Teacher for a pool of spiking neurons.");
    SpikingTeacher*TmpObj = new SpikingTeacher;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) SpikingTeacher::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticAnalogCbSynapse::classInfo ) {
    StaticAnalogCbSynapse::classInfo =registerCsimClass("StaticAnalogCbSynapse","");
    StaticAnalogCbSynapse*TmpObj = new StaticAnalogCbSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticAnalogCbSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("E",E,READWRITE,double,1,-1e30,1e30,"","Equilibrium potential given by the Nernst equation.");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,-1e30,1e30,"","The noise of our analog synapses");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticAnalogSynapse::classInfo ) {
    StaticAnalogSynapse::classInfo =registerCsimClass("StaticAnalogSynapse","A synapse which transmitts analog values (no dynamics).");
    StaticAnalogSynapse*TmpObj = new StaticAnalogSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticAnalogSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Inoise",Inoise,READWRITE,float,1,-1e30,1e30,"","The noise of our analog synapses");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticGlutamateSynapse::classInfo ) {
    StaticGlutamateSynapse::classInfo =registerCsimClass("StaticGlutamateSynapse","A Glutamate synapse with no synaptic short time dynamics, i.e. no depression and no facilitation.");
    StaticGlutamateSynapse*TmpObj = new StaticGlutamateSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticGlutamateSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("tau_nmda",tau_nmda,READWRITE,float,1,0,100,"sec","The NMDA time constant $\\tau_{nmda}$");
    REGFIELD("Mg_conc",Mg_conc,READWRITE,float,1,-1e30,1e30,"mMol","Mg-concentration for voltage-dependence of NMDA-channel in");
    REGFIELD("E_nmda",E_nmda,READWRITE,float,1,-1e30,1e30,"V","Reversal Potential for NMDA-Receptors");
    REGFIELD("E_ampa",E_ampa,READWRITE,float,1,-1e30,1e30,"V","Reversal Potential for AMPA-Receptors");
    REGFIELD("fact_nmda",fact_nmda,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.");
    REGFIELD("fact_ampa",fact_ampa,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.");
    REGFIELD("psr_nmda",psr_nmda,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) for nmda channels.");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticGlutamateSynapseSynchan::classInfo ) {
    StaticGlutamateSynapseSynchan::classInfo =registerCsimClass("StaticGlutamateSynapseSynchan","A STDP synapse with no synaptic short time dynamics, i.e. no depression and no facilitation.");
    StaticGlutamateSynapseSynchan*TmpObj = new StaticGlutamateSynapseSynchan;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticGlutamateSynapseSynchan::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("fact_nmda",fact_nmda,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the NMDA-impact.");
    REGFIELD("fact_ampa",fact_ampa,READWRITE,float,1,-1e30,1e30,"","The impact of NMDA-type channels is a fraction of the impact of the normal (AMPA) channels. This variable is multiplied to the weight to get the AMPA-impact.");
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticSpikingCbSynapse::classInfo ) {
    StaticSpikingCbSynapse::classInfo =registerCsimClass("StaticSpikingCbSynapse","A static spike transmitting synapse (no synaptic dynamics).");
    StaticSpikingCbSynapse*TmpObj = new StaticSpikingCbSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticSpikingCbSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("E",E,READWRITE,double,1,-1e30,1e30,"","Equilibrium potential given by the Nernst equation.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticSpikingSynapse::classInfo ) {
    StaticSpikingSynapse::classInfo =registerCsimClass("StaticSpikingSynapse","A static spike transmitting synapse (no synaptic dynamics).");
    StaticSpikingSynapse*TmpObj = new StaticSpikingSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticSpikingSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!StaticStdpSynapse::classInfo ) {
    StaticStdpSynapse::classInfo =registerCsimClass("StaticStdpSynapse","A STDP synapse with no synaptic short time dynamics, i.e. no depression and no facilitation.");
    StaticStdpSynapse*TmpObj = new StaticStdpSynapse;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) StaticStdpSynapse::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("back_delay",back_delay,READWRITE,float,1,-1e30,1e30,"sec","Delay of dendritic backpropagating spike (the synapse sees the postsynaptic spike delayed by back_delay");
    REGFIELD("tauspost",tauspost,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan. See Froemke and Dan (2002). Spike-timing-dependent synaptic modification induced by natural spike trains. Nature 416 (3/2002).");
    REGFIELD("tauspre",tauspre,READWRITE,float,1,-1e30,1e30,"","Used for extended rule by Froemke and Dan.");
    REGFIELD("taupos",taupos,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of positive learning window for STDP.");
    REGFIELD("tauneg",tauneg,READWRITE,float,1,-1e30,1e30,"","Timeconstant of exponential decay of negative learning window for STDP.");
    REGFIELD("dw",dw,READWRITE,float,1,-1e30,1e30,"","Missing description.");
    REGFIELD("STDPgap",STDPgap,READWRITE,float,1,-1e30,1e30,"","No learning is performed if $|Delta| = |t_{post}-t_{pre}| < STDPgap$.");
    REGFIELD("activeSTDP",activeSTDP,READWRITE,int,1,-1e30,1e30,"","Set to 1 to activate STDP-learning. No learning is performed if set to 0.");
    REGFIELD("useFroemkeDanSTDP",useFroemkeDanSTDP,READWRITE,int,1,-1e30,1e30,"","activate extended rule by Froemke and Dan (default=1)");
    REGFIELD("Wex",Wex,READWRITE,float,1,-1e30,1e30,"","The maximal/minimal weight of the synapse");
    REGFIELD("Aneg",Aneg,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the negative exponential learning window.");
    REGFIELD("Apos",Apos,READWRITE,float,1,-1e30,1e30,"","Defines the peak of the positive exponential learning window.");
    REGFIELD("mupos",mupos,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative positive update: $dw = (Wex-W)^{mupos} * Apos * exp(-Delta/taupos)$. Set to 0 for basic update. See Guetig, Aharonov, Rotter and Sompolinsky (2003). Learning input correlations through non-linear asymmetric Hebbian plasticity. Journal of Neuroscience 23. pp.3697-3714.");
    REGFIELD("muneg",muneg,READWRITE,float,1,-1e30,1e30,"","Extended multiplicative negative update: $dw = W^{mupos} * Aneg * exp(Delta/tauneg)$. Set to 0 for basic update.");
    REGFIELD("tau",tau,READWRITE,float,1,0,100,"sec","The synaptic time constant $\\tau$");
    REGFIELD("W",W,READWRITE,float,1,-1e30,1e30,"","The weight (scaling factor, strenght, maximal amplitude) of the synapse");
    REGFIELD("delay",delay,READWRITE,float,1,0,1,"sec","The synaptic transmission delay");
    REGFIELD("psr",psr,READONLY,double,1,-1e30,1e30,"","The psr (postsynaptic response) is the result of whatever computation is going on in a synapse.");
    REGFIELD("steps2cutoff",steps2cutoff,READONLY,int,1,-1e30,1e30,"","Missing description.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!Traubs_HH_K_Channel::classInfo ) {
    Traubs_HH_K_Channel::classInfo =registerCsimClass("Traubs_HH_K_Channel","Traubs modified version of Hodgkin and Huxley's fast potassium (K) channel for AP generation.");
    Traubs_HH_K_Channel*TmpObj = new Traubs_HH_K_Channel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) Traubs_HH_K_Channel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!Traubs_HH_Na_Channel::classInfo ) {
    Traubs_HH_Na_Channel::classInfo =registerCsimClass("Traubs_HH_Na_Channel","Traubs modified version of Hodgkin and Huxley fast sodium (Na) channel for AP generation.");
    Traubs_HH_Na_Channel*TmpObj = new Traubs_HH_Na_Channel;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) Traubs_HH_Na_Channel::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Gbar",Gbar,READWRITE,float,1,0,1,"S","The maximum conductance of the channel;");
    REGFIELD("Erev",Erev,READWRITE,float,1,-1,1,"V","The reversal potential $E_{rev}$ of the channel");
    REGFIELD("g",g,READONLY,double,1,0,1,"S","The coductance $g(t,V_m)$ of the channel");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!TraubsHHNeuron::classInfo ) {
    TraubsHHNeuron::classInfo =registerCsimClass("TraubsHHNeuron","Conductance based spiking neuron using Traubs modified HH model.");
    TraubsHHNeuron*TmpObj = new TraubsHHNeuron;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) TraubsHHNeuron::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("Vthresh",Vthresh,READWRITE,float,1,-10,100,"V","If $V_m$ exceeds $V_{thresh}$ a spike is emmited.");
    REGFIELD("Vreset",Vreset,READWRITE,float,1,-1,1,"V","The voltage to reset $V_m$ to after a spike.");
    REGFIELD("doReset",doReset,READWRITE,int,1,0,1,"flag","Flag which determines wheter $V_m$ should be reseted after a spike");
    REGFIELD("Trefract",Trefract,READWRITE,float,1,0,1,"sec","Length of the absolute refractory period.");
    REGFIELD("nummethod",nummethod,READWRITE,int,1,0,1,"flag","Numerical method for the solution of the differential equation: Exp. Euler = 0, Crank-Nicolson = 1");
    REGFIELD("type",type,READWRITE,int,1,-1e30,1e30,"","Type (e.g. inhibitory or excitatory) of the neuron");
    REGFIELD("Cm",Cm,READWRITE,float,1,0,1,"F","The membrane capacity $C_m$");
    REGFIELD("Rm",Rm,READWRITE,float,1,0,1e30,"Ohm","The membrane resistance $R_m$");
    REGFIELD("Em",Em,READONLY,double,1,-1,1,"V","The reversal potential of the leakage channel");
    REGFIELD("Vresting",Vresting,READWRITE,float,1,-1,1,"V","The resting membrane voltage.");
    REGFIELD("Vinit",Vinit,READWRITE,float,1,-1,1,"V","Initial condition for$V_m$ at time $t=0$.");
    REGFIELD("VmScale",VmScale,READWRITE,float,1,0,1e5,"V","Defines the difference between Vresting and the Vthresh for the calculation of the iongate tables and the ionbuffer Erev.");
    REGFIELD("Vm",Vm,READONLY,double,1,-1e30,1e30,"V","The membrane voltage");
    REGFIELD("Inoise",Inoise,READWRITE,float,1,0,1,"W^2","Variance of the noise to be added each integration time constant.");
    REGFIELD("Iinject",Iinject,READWRITE,float,1,-1,1,"A","Constant current to be injected into the CB neuron.");
    REGFIELD("Isyn",Isyn,READONLY,double,1,-1e30,1e30,"","synaptic input current");
    REGFIELD("Gsyn",Gsyn,READONLY,double,1,-1e30,1e30,"","synaptic input conductance");
    REGFIELD("nIncoming",nIncoming,READONLY,int,1,-1e30,1e30,"","Number of incoming synapses");
    REGFIELD("nOutgoing",nOutgoing,READONLY,int,1,-1e30,1e30,"","Number of outgoing synapses");
    REGFIELD("nBuffers",nBuffers,READONLY,int,1,-1e30,1e30,"","Number of ion buffers");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of channels");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!TriangularAnalogFilter::classInfo ) {
    TriangularAnalogFilter::classInfo =registerCsimClass("TriangularAnalogFilter","Implementation of an analog low-pass filter with a triangular kernel.");
    TriangularAnalogFilter*TmpObj = new TriangularAnalogFilter;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) TriangularAnalogFilter::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("nKernelLength",nKernelLength,READWRITE,int,1,-1e30,1e30,"","The length of the kernel.");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of Input channels to filter.");
    #undef REGFIELD
    delete TmpObj;
  }

  if (!UserAnalogFilter::classInfo ) {
    UserAnalogFilter::classInfo =registerCsimClass("UserAnalogFilter","Implementation of an analog low-pass filter with a user-defined kernel (maximum length = 10).");
    UserAnalogFilter*TmpObj = new UserAnalogFilter;
    #define REGFIELD(_n,_f,_a,_t,_s,_l,_u,_un,_d) UserAnalogFilter::classInfo->registerField((char *)(TmpObj),_n,&(TmpObj->_f),_a,_s,_l,_u,_un,_d);
    REGFIELD("m_weights",m_weights,READWRITE,double *,10,-1e30,1e30,"","User-defined weights.");
    REGFIELD("nKernelLength",nKernelLength,READWRITE,int,1,-1e30,1e30,"","The length of the kernel.");
    REGFIELD("nChannels",nChannels,READONLY,int,1,-1e30,1e30,"","Number of Input channels to filter.");
    #undef REGFIELD
    delete TmpObj;
  }


}
