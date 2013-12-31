% INSTALL will install the available packages.
%
%   It checks wich subdirectories (csim, circuits, learning) 
%   are available and calls install.m for each available
%   subdirectory. 


% potential packages to install
dirs = { 'csim' 'circuits' 'learning' 'utilities' 'develop' 'inputs' 'research' };

% save current directory
rwd=pwd;

% go through all possible directories
for d=1:length(dirs)
  if exist(dirs{d}) == 7
    cd(dirs{d});
    if( exist('./install.m','file') )
      install;
    end
    cd(rwd);
  end
end
