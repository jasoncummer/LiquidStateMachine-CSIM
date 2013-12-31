%
% definitions for csim_net.c
%

%
% fundamental parameters
%

SPIKING = 1;
SIGMOID = 2;

% this sets the type of the liquid !!!
LIQUID = SPIKING;

%
% neuron models and flags
%
INPUT            =     1;    %->csim_net.c
SPIKE_INPUT      = INPUT;    %->csim_net.c
SRM_ABS          =     2;    %->csim_net.c
SRM_LTM          =     4;    %->csim_net.c
SRM_STM          =     8;    %->csim_net.c
LIF_NEURON       =    16;    %->csim_net.c
OUTPUT           =    32;    %->csim_net.c
ANALOG_INPUT     =    64;    %->csim_net.c
SIG_NEURON       =   128;
KHEP_PROX_INPUT  =   256;
KHEP_MOT_INPUT   =   512;
KHEP_LIGHT_INPUT =  1024;
TRACKER_INPUT    =  2048;

OUTPUT_bitnmr = log(OUTPUT)/log(2)+1;

%
% synapse models and flags
%
STATIC           =       1;  %->csim_net.c
MARKRAM          =       2;  %->csim_net.c
DYNAMIC          = MARKRAM;  %->csim_net.c
STOCHASTIC       =       4;  %->csim_net.c
DETERMININSTIC   =       8;  %->csim_net.c
NOISY            =      16;  %->csim_net.c
LEARN            =      64;  %->csim_net.c
ANA_DYN_SYN      =     128;
ANALOG           =     256;
ANA_STAT_SYN     =  ANALOG;
ANALOG_SYNAPSE   =  ANALOG;
SPIKE_SYNAPSE    =     512;

%
% some constant definitions
%
INH = 1;
EXC = 2;

%
% synapse types
%
II = (INH-1) * 2 + INH;
IE = (INH-1) * 2 + EXC;
EI = (EXC-1) * 2 + INH;
EE = (EXC-1) * 2 + EXC;
syn_type_str = { 'II' 'IE' 'EI' 'EE' };

%
% regions
%
ALL=[];

%
% for reading out the state
%

READ_CURRENT   = 1;
READ_POTENTIAL = 2;
PROBABILITY    = 1;
FRACTION       = 2;

%
%
% for learning
%
NO_LEARN       =   0; %->csim_net.c
ON_SHOT_LEARN  =   1; %->csim_net.c
BATCH_LEARN    =   2; %->csim_net.c
LEARN_TRACE    = 128; %->csim_net.c

% used to define which synapses are learnable
EXC_INPUT_SYN  = 1;
INH_INPUT_SYN  = 2;
II_LIQUID_SYN  = 3;
IE_LIQUID_SYN  = 4;
EI_LIQUID_SYN  = 5;
EE_LIQUID_SYN  = 6;
EXC_OUTPUT_SYN = 7;
INH_OUTPUT_SYN = 8;

%
% for plotting and printing
%
global VERBOSE_LEVEL
global PLOTTING_LEVEL

if isempty(VERBOSE_LEVEL), VERBOSE_LEVEL   = 2; end;
if isempty(PLOTTING_LEVEL), PLOTTING_LEVEL = 2; end;
