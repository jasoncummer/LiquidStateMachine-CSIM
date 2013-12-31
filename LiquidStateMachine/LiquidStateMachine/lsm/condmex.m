function r=condmex( tgt, dir, cmd );

rwd=pwd;
cd(dir);

ltgt = sprintf('%s/%s',pwd,tgt);

if exist(ltgt) == 3
  fprintf('%s exists. No compilation necessary.\n',tgt);
  if all(mexext=='mexw32')
    system(sprintf('copy %s.mexw32 %s.dll',tgt,tgt)); 
  end
  r=0;
else
  fprintf('Compiling %s ...',tgt);
  eval(cmd);
  if exist(ltgt) == 3
    fprintf('\b\b\b\b. Success.\n');
    if all(mexext=='mexw32')
        system(sprintf('copy %s.mexw32 %s.dll',tgt,tgt)); 
    end
    r=1;
  else
    fprintf('\b\b\b\b. FAILED.\n');
    r=-1;
  end
end

cd(rwd);
