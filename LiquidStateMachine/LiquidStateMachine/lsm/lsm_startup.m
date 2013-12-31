if exist('LSM_STARTUP') ~= 1

  LSMROOT='';
  st=which('lsm_startup');
  if isempty(LSMROOT)
    LSMROOT=st(1:strfind(st,'\lsm_startup.m')-1);
  end
  if isempty(LSMROOT)
    LSMROOT=st(1:strfind(st,'/lsm_startup.m')-1);
  end

  %%
  %% get full path for LSMROOT and print it.
  %%
  cw=pwd; cd(LSMROOT);
  clear LSMROOT
  clear global LSMROOT
  global LSMROOT
  LSMROOT=pwd;
  cd(cw);
  
  close all   % delete all figure windows

  lsm_path(LSMROOT);

  clear

  global_definitions               % load some constant definitions
  
  VERBOSE_LEVEL  = 2;              % the higher the more is printed at stdout 
  PLOTTING_LEVEL = 1;              % the gigher the more plots will appear
  
  global LSM_STARTUP LSMROOT
  LSM_STARTUP = 1;
  fprintf('\nPaths initialized for LSMROOT=%s\n\n',LSMROOT);
  
end
