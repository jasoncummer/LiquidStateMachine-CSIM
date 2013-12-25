function parallel=can_run_parallel(PAR)

parallel=0;
if PAR
  if pm_available
    vmm=pmmembvm(0);
    if ~isempty(vmm)
      if vmm(1) > 0
	parallel = 1;
      end
    end
  end
end
