%
% this is *super tolles* demo
%

clear all
rand('state',8931180);
randn('state',8931180);

addpath('..');

%
% make sure that we start from scratch
%
csim('destroy');

%
% the passive parameters
%
Em = 10.61299992*1e-3;    % Volt
Rm = 424.4135437*1e3;     % Ohm
Cm = 0.007853974588*1e-6; % Farad
Vresting = -0.04;         % Volt

%
% create a CbNeuron
%
n = csim('create','CbNeuron');

%
% set the parameters of this neuron (in the future should be able with one command line)
%
csim('set',n,'Cm',Cm);
csim('set',n,'Rm',Rm);
csim('set',n,'Vresting',Vresting);
csim('set',n,'Vreset',Vresting);
csim('set',n,'Vinit',Vresting);
csim('set',n,'Trefract',0.005);
csim('set',n,'Inoise',0.00*1e-6);
csim('set',n,'Iinject',0.00*1e-6);
csim('set',n,'Vthresh',0.02);
csim('set',n,'doReset',0);

%
% create a H&H K+ channel
%
k=csim('create','HH_K_Channel');
csim('set',k,'Gbar',0.2827430964*1e-3);
csim('set',k,'Erev',-11.99979305*1e-3+Vresting);

%
% create a H&H Na++ channel
%
na=csim('create','HH_Na_Channel');
csim('set',na,'Gbar',0.9424769878*1e-3);
csim('set',na,'Erev',0.1150009537+Vresting);

%
% now we have to cennect the objects
%
csim('connect',n,k);   % make the K channel a part of the neuron (in the future we will have a 'insert')
csim('connect',k,n);   % let the K channel now that it is a part of the neuron

csim('connect',n,na);  % make the Na channel a part of the neuron
csim('connect',na,n);

%
% create an analog input neuron an a synapse
%
ain=csim('create','AnalogInputNeuron');
s=csim('create','StaticAnalogSynapse');

%
% connect them: NOTE: csim('connect',destination,source
%
csim('connect',s,ain); % the output of the input device goes to the syn
csim('connect',n,s);   % the syn feeds into our neuron

%
% now we want to record some values during the simulation:
%
r=csim('create','MexRecorder');

%
% tell the recorder which fields to record
%
csim('connect',r,n,'Vm');
csim('connect',r,na,'g');
csim('connect',r,k,'g');
csim('connect',r,s,'psr');
csim('connect',r,n,'spikes');

%
% set parameters regarding the simulation control
%
dt=2e-5;
dtr=max(1e-4,dt);
Tsim=1;
csim('set',r,'dt',dtr);           % how offten the recorde sould store the values
csim('set','dt',dt);              % the integration time constant
csim('set','randSeed',randseed);  % the randSeed 

%
% define an input which will be fedd to out input object
%
input(1).spiking = 0;                                 % its analog
input(1).dt      = 1e-3;                              % time base
input(1).idx     = ain;                               % where to send the input to
t=0:input(1).dt:Tsim;
input(1).data    = 0.04e-6*(2*rand(size(t))-1);  % a sine modulted current

%
% now do the simulation
%
csim('reset');


tic
output=csim('simulate',Tsim,input);
fprintf('%g times faster then real time\n',Tsim/toc);


%
% plot the results
%
figure(1); clf reset;
subplot(2,1,1);

t=[dtr:dtr:Tsim];

plot(t,output{1}.channel(1).data); hold on;
line([0 Tsim],csim('get',n,'Vresting')*[1 1],'Color','k');
xlabel('time [sec]');
ylabel('Vm (Volts)');
axis tight

subplot(2,1,2);
plot(t,output{1}.channel(2).data,'r'); hold on;
plot(t,output{1}.channel(3).data,'b'); hold on;
legend('g_{Na}(t) (Siemens)','g_K(t) (Siemens)');
xlabel('time [sec]');


fprintf('nSpikes=%i\n',length(output{end}.times));

%net = csim('export');
%
%save test
%
%clear all
%
%load test
%delete test.mat
%
%csim('import',net);

csim('set','dt',dt/10);

csim('reset');

tic
csim('simulate',Tsim,input);
out1=csim('get',r,'traces');

fprintf('%g times faster then real time\n',Tsim/toc);

figure(2); clf reset;
subplot(2,1,1);

t=[dtr:dtr:Tsim];

plot(t,out1.channel(1).data); hold on;
line([0 Tsim],csim('get',n,'Vresting')*[1 1],'Color','k');
xlabel('time [sec]');
ylabel('Vm (Volts)');
axis tight

subplot(2,1,2);
plot(t,out1.channel(2).data,'r'); hold on;
plot(t,out1.channel(3).data,'b'); hold on;
legend('g_{Na}(t) (Siemens)','g_K(t) (Siemens)');
xlabel('time [sec]');

fprintf('nSpikes=%i\n',length(out1.channel(5).data));


