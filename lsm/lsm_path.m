function lsm_path(root)
%Add LSM relevant directories to the matlab path. 
%
%  Syntax
%
%    lsm_path(root);
%
%  Description
%
%    lsm_path(root) adds several sub directories of root to the matlab
%    search path. This function is used if starting the lsm software
%    tools via lsm_startup.
%
%  See also
%
%    lsm_startup

% Author: Thomas Natschlaeger, tnatschl@igi.tu-graz.ac.at
% $Authore$, $Revsison$, $Date: 2006/06/09 16:53:08 $
  
% Internal Comment: use the command
%
% find . -name '*.m' ! -path '*/research/*' ! -path '*/www/*' ! -path '*/demos/*' ! -path '*/develop/*' | sed -e s+'/[^/]*$'++ -e s+'/@.*$'++ | sort -u
%
% to find a list of directories necessary to add to the matlab search
% path to be able to access all m-files.
  
list = { 
    'csim', ...
    'circuits', ...
    'inputs', ...
    'learning', ...
    'learning/svm_toolbox', ...
    'gui', ...
    'utilities', ...
    'utilities/parallel_matlab', ...
    'utilities/mutual_information'
};

for i=1:length(list)
  p=[root '/' list{i}];
 if exist(p) == 7
    addpath(p);
  end
end


