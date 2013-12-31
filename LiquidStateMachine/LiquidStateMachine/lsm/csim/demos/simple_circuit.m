%
% This is a short demo which shows how to use CSIM
%
% We implement a network with one spiking input neuron,
% one analog input neuron, one exzitatory and
% one inhibitory neuron with simple connections between them:
%
%                          syn4
%  analog input(4) ------------------------+
%                                          |
%                   syn1           syn2    v
%  spike input(1) ------> exz(2) ------> inh(3) -+
%                           o                    |
%                           |        syn3        |
%                           +--------------------+
%               
% Author: Thomas Natschlaeger, 15/11/2001
%

clear all

addpath('..');

%
% define the neurons
%
%
% simulation parameters
%
dt_sim  = 1e-4;  % integration time step [sec]
Tsim    = 0.6;   % simulation time [sec]
dt_out  = 0.005; % intervals at which VMs and PSCs are recorded [sec]
noise   = 0.00;   % the amount of noise [e.g. nA^2]
mySeed  = 314159;

csim('destroy');
csim('set','dt',dt_sim);
csim('set','randSeed',mySeed);

% the input neuron
n1 = csim('create','SpikingInputNeuron');

% a leaky integrate and fire neuron
n2 = csim('create','LifNeuron');  
csim('set',n2,'Vthresh',0.015);  % threshold  
csim('set',n2,'Trefract',0.003); % refractory period
csim('set',n2,'Cm',0.03);        % tau_m = Cm * Rm
csim('set',n2,'Vreset',0.005);   % V_reset
csim('set',n2,'Iinject',0.002);  % I_back
csim('set',n2,'Vinit',0.001);    % V_init
csim('set',n2,'Rm',1.0);
csim('set',n2,'Inoise',noise);
csim('set',n2','Vresting',0);

% another leaky integrate and fire neuron (index 3)
n3 = csim('create','LifNeuron');      % type
csim('set',n3,'Vthresh',0.01,'Trefract',0.002,'Cm',0.05,'Vreset',0.0,'Iinject',0.0,'Vinit',0.0,'Rm',1.0,'Inoise',noise,'Vresting',0); % threshold  

% an analog input wave form unit
n4 = csim('create','AnalogInputNeuron');

%
% set up the connections
%

% exzitatory synapse 1 (a static one) connects neuron 1 (the input) to neuron 2;
syn1 = csim('create','StaticSpikingSynapse'); % type
csim('set',syn1,'W',0.5); % weight
csim('set',syn1,'delay',0.001); % delay
csim('set',syn1,'tau',0.003); % tau_s

csim('connect',n2,n1,syn1);

% exzitatory synapse 2 (a dynamic one) connects neuron 2 to neuron 3;
syn2 = csim('create','DynamicSpikingSynapse'); % type
csim('set',syn2,'W',0.5);% weight
csim('set',syn2,'delay',0.001);% delay
csim('set',syn2,'tau',0.003);% tau_s
csim('set',syn2,'U',0.2);% U
csim('set',syn2,'D',1.0);% D
csim('set',syn2,'F',0.3);% F
csim('set',syn2,'u0',0.2);
csim('set',syn2,'r0',1.0);

csim('connect',n3,n2,syn2);

% inhibitory synapse 3 (a dynamic one) connects neuron 3 back to neuron 2;
syn3 = csim('create','DynamicSpikingSynapse'); % type
csim('set',syn3,'W',-0.1);% weight
csim('set',syn3,'delay',0.0);% delay
csim('set',syn3,'tau',0.03);% tau_s
csim('set',syn3,'U',0.7);% U
csim('set',syn3,'D',0.1);% D
csim('set',syn3,'F',1.0);% F
csim('set',syn3,'u0',0.7);
csim('set',syn3,'r0',1.0);

csim('connect',n2,n3,syn3);
			       
% exzitatory synapse 4 (an analog onw) connects neuron 3 back to neuron 2;
syn4 = csim('create','StaticAnalogSynapse');
csim('set',syn4,'W',0.018); % weight
csim('set',syn4,'Inoise',noise);% noise
csim('connect',n3,n4,syn4);


%
% We want to plot the Vm's and PSC's of nrns 2 and 3 and of all syns
%

rec_psc = csim('create','MexRecorder');
csim('set',rec_psc,'dt',dt_out);
csim('connect',rec_psc,[syn1 syn2 syn3 syn4],'psr');

rec_vm = csim('create','MexRecorder');
csim('set',rec_vm,'dt',dt_out);
csim('connect',rec_vm,[ n2 n3 ],'Vm');

rec_sp = csim('create','MexRecorder');
csim('connect',rec_sp,[ n1 n2 n3 ],'spikes');

%
% Set up the input
%
input(1).spiking = 1;
input(1).idx     = n1;
input(1).data    = cumsum([0.00851522 0.0374612 0.0304122 0.00513629 0.0011292 0.00487756 0.0820378 0.0198986 ...
                           0.0343706 0.016819 0.0179849 0.0783438 0.0407395 0.0287782 0.028933 0.0271107 ...
			   0.0356939 0.00673942 0.0278204 0.0199695]);
input(1).dt      = 0;

input(2).spiking = 0;
input(2).idx     = n4;
input(2).dt      = 5e-3;
input(2).data    = 1*(1+sin(2*pi*6*[0:input(2).dt:Tsim])); 

%
% run the simulation
%
tic;
csim('reset');
csim('simulate',Tsim/3,input);
csim('simulate',Tsim/3,input);
csim('simulate',Tsim/3,input);
vm = csim('get',rec_vm,'traces');
psc = csim('get',rec_psc,'traces');
spikes = csim('get',rec_sp,'traces');
toc

%
% now we plot the output
%
figure(1); clf reset;

subplot(4,1,1); cla reset;

t=spikes.channel(1).data;
line([t; t],1+[0.3; -0.3]*ones(1,length(t)),'Color','k'); hold on

plot([0:input(2).dt:Tsim],2+input(2).data,'r-');

title('input');

subplot(4,1,2); cla reset;

t = spikes.channel(2).data;
plot(t,1*ones(1,length(t)),'b.'); hold on
line([t; t],1+[0.3; -0.3]*ones(1,length(t)),'Color','b'); 

t = spikes.channel(3).data;
plot(t,2*ones(1,length(t)),'g.'); hold on
line([t; t],2+[0.3; -0.3]*ones(1,length(t)),'Color','g'); 
set(gca,'Xlim',[0 Tsim],'Ylim',[0.5 2.5]);
%legend('neuron 1','neuron 2');
title('spike times');

subplot(4,1,3); cla reset;
ldt=vm.channel(1).dt;
t=ldt:ldt:Tsim;
plot(t,vm.channel(1).data,'b'); hold on
plot(t,vm.channel(2).data,'g'); hold on
axis tight
set(gca,'Xlim',[0 Tsim]);
legend('neuron 1','neuron 2');
title('membrane voltage');

subplot(4,1,4); cla reset;
ldt=csim('get',rec_psc,'dt');
t=ldt:ldt:Tsim;
plot(t,psc.channel(1).data,'k'); hold on
plot(t,psc.channel(2).data,'b'); hold on
plot(t,psc.channel(3).data,'g'); hold on
plot(t,psc.channel(4).data,'r'); hold on
axis tight
set(gca,'Xlim',[0 Tsim]);
legend('syn 1','syn 2','syn 3','syn 4');
xlabel('time [sec]');
title('synaptic potentials');
