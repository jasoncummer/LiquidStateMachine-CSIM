
if exist('csim') ~= 3
  if isunix
    !make
  elseif ispc
    !nmake -f Makefile.win
  else
    disp('Unsupported operating system! Terminated.'); return;
  end
  v=csim('version');
  fprintf('\b\b\b\b. Done.\nCSIM %s successfully compiled.\n',v);
else
  fprintf('csim.%s exists. No compilation necessary.\n',mexext);  
end

fprintf('CSIM *succesfully* installed.\n\n');
