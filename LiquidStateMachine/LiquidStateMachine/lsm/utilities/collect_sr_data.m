function [Rout,Sout] = collect_sr_data(nmc,Sin,nS,Tsim,R2X,pm,si,rs,GUI,optTI,isSlave)

global VERBOSE_LEVEL;

if nargin < 2, error('Must at least specifiy the nmc and the stimuli'); end
if nargin < 3, nS    = []; end
if nargin < 4, Tsim  = []; end
if nargin < 5, R2X   = []; end
if nargin < 6, pm    = []; end
if nargin < 7, si    = []; end
if nargin < 8, rs    = []; end
if nargin < 9, GUI   = []; end
if nargin < 10, optTI   = []; end
if nargin < 11, isSlave = 0; end

if isempty(nS),    nS   = 50; end
if isempty(Tsim),  Tsim = -1; end

if isempty(pm), pm = 1; end
if isempty(si), si = 1; end
if isempty(GUI), GUI = 0; end

if isempty(optTI), optTI = 0; end

global PLOTTING_LEVEL

if ~can_run_parallel(pm)

  if ~isstruct(Sin)
    verbose(0,'running %i simulations with input distribution %s\n',nS,class(Sin));
  else
    nS = length(Sin);
    verbose(0,'running %i simulations with given stimuli (%s)\n',nS,inputname(2));
  end

  if isempty(rs)
    rs = ceil(rand(1,nS)*1e6);
  end

  tic;
  for i=1:nS
    % generate input
    if ~isstruct(Sin)
      Sout(i) = generate(Sin,Tsim,si+i-1);
    else
      Sout(i) = Sin(i);
    end

    % check simulation time
    if Tsim == -1, Tmax = Sout(i).info(1).Tstim; else Tmax = Tsim; end
    if Tmax < 0, error('Neither stimulus determines Tsim nor is Tsim explicitly given.'); end

    % progress info
    verbose(1,'-');

    % run simulation
    reset(nmc,rs(i));

    R = simulate(nmc,Tmax,Sout(i),[]);

    if optTI == 1
      R{1} = channel2timeindex(R{1});
    end

    if ~isempty(R2X)
      tmpv=VERBOSE_LEVEL; VERBOSE_LEVEL=-1;
      Rout(i) = feval(R2X{1},R{1},Sout(i),R2X{2:end});
      VERBOSE_LEVEL=tmpv;
    else
      if optTI
        Rout(i).spiketimes = double(R{1}.spiketimes);
        Rout(i).channel    = double(R{1}.channel);
        Rout(i).index      = double(R{1}.index);
        Rout(i).Tsim       = double(R{1}.Tsim);
      else
        Rout(i) = R{1};
      end
    end

    % make some plot if required
    if PLOTTING_LEVEL > 1, plot_pair(Sout(i),R); end

    % progress info
    if rem(i,50)==0
      verbose(1,'\b. %i (%i sec, %i sec to run)\n',i,round(toc),round(toc/i*(nS-i)));
    else
      verbose(1,'\b.');
    end

  end
  verbose(0,'\n');

else
  %
  % Run it parallel with 'blockSize' stimulations per block.
  % We choose 'blockSize' such that there are roughly
  % 4 * #CPUs blocks to achieve a reasonable graining.
  %  
  % Save common data. Usually one would send this by means of the
  % comdata and comarg fields. However  
  %   1) the pm toolbox is buggy and objects like nmc and Sin 
  %      do not work
  %   2) nmc may become very large and using comdata would double
  %      the required memory
  %

  wd = pwd;
  cd(getenv('HOME'));
  home=pwd;
  cd(wd);
  currentDirectory=[home '/' wd(length(home)+2:end)];

  if ~isstruct(Sin)
    
    [nBlocks,blockSize,nCPUs]=blocksize(nS);
  
    verbose(0,'running %i stimulations with stim. dist. %s in PARALLEL (%i CPUs, %i blocks, size %i)\n',...
	nS,class(Sin),nCPUs,nBlocks,blockSize);

    save commdata.mat Sin nmc R2X optTI
    
    coll_fun=pmfun;
    coll_fun.expr    = [ ...
	  'rand(''state'',randSeed(1,2));'...
	  'randn(''state'',randSeed(1,2));'...
	  '[R,S]=collect_sr_data(nmc,Sin,nStimuli,Tsim,R2X,0,startIndex,randSeed,[],optTI,1);'
      ];
    coll_fun.argin   = { 'randSeed' 'startIndex' };
    coll_fun.datain  = { 'GETBLOC(1)' 'GETBLOC(2)' };
    coll_fun.argout  = { 'R' 'S' };
    coll_fun.dataout = { 'SETBLOC(1)' 'SETBLOC(2)' };
    coll_fun.comarg  = { 'nStimuli'  'Tsim' };
    coll_fun.comdata = {  blockSize   Tsim  };
    coll_fun.prefun  = [ 'cd ' currentDirectory '; rehash; load commdata.mat; PLOTTING_LEVEL=0; VERBOSE_LEVEL=1;'];
    
    randSeed = ceil(rand(nS,2)*1e6);

    startIndex = 1:blockSize:nS;
    
    indsi1=createinds(randSeed,[blockSize 2]);
    indsi2=createinds(startIndex,[1 1]);
    indso =createinds([1:nS],[1 blockSize]);
    
    coll_fun.blocks = pmblock('src',[indsi1 indsi2],'dst',[indso indso]);
    
    [err,Rout,Sout] = dispatch(coll_fun,0,{ randSeed startIndex },[],[],'saveinterv',nBlocks*2,'gui',GUI,'logfile','log.collect_sr_data');
    
    delete commdata.mat
    
  else
    nS = length(Sin);
    [nBlocks,blockSize,nCPUs]=blocksize(nS);
  
    verbose(0,'running %i stimulations with given stimulus (%s) in PARALLEL (%i CPUs, %i blocks, size %i)\n',...
	nS,inputname(2),nCPUs,nBlocks,blockSize);
    
    save commdata.mat nmc Tsim R2X optTI
    
    coll_fun=pmfun;
    coll_fun.expr    = [ ...
	  'rand(''state'',randSeed(1,1));'...
	  'randn(''state'',randSeed(1,2));'...
	  '[R,S]=collect_sr_data(nmc,Sin,[],Tsim,R2X,0,[],randSeed,[],optTI,1);'
      ];
    coll_fun.argin   = { 'Sin'    'randSeed'   };
    coll_fun.datain  = { 'GETBLOC(1)' 'GETBLOC(2)' };
    coll_fun.argout  = { 'R' 'S' };
    coll_fun.dataout = { 'SETBLOC(1)' 'SETBLOC(2)' };
    coll_fun.comarg  = { };
    coll_fun.comdata = { };
    coll_fun.prefun  = [ 'cd ' currentDirectory '; rehash; load commdata.mat; PLOTTING_LEVEL=0; VERBOSE_LEVEL=1;'];
    
    randSeed = ceil(rand(nS,2)*1e6);
    
    indsi1=createinds(Sin,[1 blockSize]);
    indsi2=createinds(randSeed,[blockSize 2]);
    indso =createinds(Sin,[1 blockSize]);
    
    coll_fun.blocks = pmblock('src',[indsi1 indsi2],'dst',[indso indso]);
    
    [err,Rout,Sout] = dispatch(coll_fun,0,{ Sin randSeed },[],[],...
                               'gui',GUI,'logfile','log.collect_sr_data','saveinterv',nBlocks*2);
    
    delete commdata.mat
  end

end

if ~ isSlave
  if optTI
    for i=1:length(Rout)
      Rout(i).spiketimes = int32(Rout(i).spiketimes);
      Rout(i).channel    = int16(Rout(i).channel);
      Rout(i).index      = int16(Rout(i).index);
    end
  end
end

