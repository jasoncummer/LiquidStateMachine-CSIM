function x=unirnd(l,u,m,n)

  if nargin < 3, m=1; end
  if nargin < 4, n=1; end
  
  x=l+(u-l)*rand(m,n);
