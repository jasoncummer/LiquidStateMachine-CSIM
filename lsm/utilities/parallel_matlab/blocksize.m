function [n,s,nCPUs]=blocksize(N,nCPUs)

if nargin < 2, nCPUs = []; end
if isempty(nCPUs), nCPUs = length(pmmembvm(0)); end

%verbose(0,'%i CPUs currently available\n',nCPUs);

for c=(floor(N/nCPUs/4)+1):-1:1
  if rem(N,c) == 0
    s = c; break;
  end
end
n = N/s;
