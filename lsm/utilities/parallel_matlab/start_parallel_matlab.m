function start_parallel_matlab(varargin)


if isempty(getenv('PM_PATH'))
  fprintf('It seems that there is NO parallel Matlab installed!\n');
  return;
end

if pmis
  fprintf('Parallel Matlab already started!\n');
  return;
end

if length(varargin) > 0
  args = varargin;
else
  args = available_hosts;
end

h     = 0;
DEBUG = 0;
GUI   = 0;
nice  = 'same';
hosts = [];
num   = [];
while length(args) > 0
  if strcmp(upper(args{1}),'DEBUG')
    DEBUG = 1;
    args = args(2:end);
  elseif strcmp(upper(args{1}),'NICE')
    nice = args{2};
    args = args(3:end);
  elseif strcmp(upper(args{1}),'GUI')
    GUI = 1;
    args = args(2:end);
  elseif ~isempty(str2num(args{1}))
    h=h+1;
    hosts{h} = sprintf('figipc%s.tu-graz.ac.at',args{1});
    num(h)   = 1;
    args = args(2:end);
    if length(args) > 0
      if isnumeric(args{1})
	num(h) = args{1};
	args = args(2:end);
      end
    end
  elseif ischar(args{1})
    h=h+1;
    hosts{h} = args{1};
    num(h)   = 1;
    args = args(2:end);
    if length(args) > 0
      if isnumeric(args{1})
	num(h) = args{1};
	args = args(2:end);
      end
    end
  end
end

hosts = hosts(find(num>0));
num   = num(find(num>0));

for i=1:length(num)
  if isempty(find(hosts{i}=='.'))
    hosts{i}=sprintf('%s.tu-graz.ac.at',hosts{i});
  end
end

if isempty(hosts)
  hosts = { 'figipc70.tu-graz.ac.at' ...
            'figipc71.tu-graz.ac.at' ...
            'figipc72.tu-graz.ac.at' ...
            'figipc73.tu-graz.ac.at' }
end

if isempty(num), num = ones(1,length(hosts)); end
if isempty(DEBUG), DEBUG = 0; end

wd = pwd;
cd(getenv('HOME'));
home=pwd;
cd(wd);
wd=[ './' wd(length(home)+2:end)];

if DEBUG
  hosts = hosts(1:min(end,2));
  num   = ones(1,length(hosts));
end

for m=1:length(hosts)
  for n=1:num(min(length(num),m))
    log_files{n,m} = sprintf('%s/log.%s.%i',wd,hosts{m},n);
  end
end

vm.wd      = wd;
vm.prio    = nice; 

vm.try     = '';
vm.catch   = '';
if DEBUG
  vm.runmode = 'fg';
else
  vm.runmode = 'bg';
end

fprintf('\n');
tids=pmopen(hosts,vm,{hosts,num,0,log_files});

fprintf('\n\nWaiting for all matlab instances to join ');
tic; nCPUs=length(pmmembvm(0));
while toc < 60 & nCPUs < sum(num)
  fprintf('-');
  pause(1);
  nCPUs=length(pmmembvm(0));
end
if nCPUs == sum(num)
  fprintf('> %i CPUs in VM 0\n\n',length(pmmembvm(0)));
else
  fprintf(' some CPUs did not join VM 0. Check for errors.');
end

%
% initialize the random generators at each node differently
%
tids=pmmembvm(0);
for i=1:length(tids)
  pmeval(tids(i),sprintf('rand(''state'',%i); randn(''state'',%i);',ceil(rand*1e6),ceil(rand*1e6)));
end

%
% initialize lsm toolbox
%
global LSMROOT
if isempty(LSMROOT)
  rel_lsmroot = '~/lsm';
  fprintf('LSMROOT not set => using ~/lsm');
else
  rel_lsmroot=['~/' LSMROOT(length(home)+2:end)];
end
tids=pmmembvm(0);
pmeval(tids,sprintf('cur=pwd; cd %s; addpath(pwd); cd(cur); lsm_startup;',rel_lsmroot));



