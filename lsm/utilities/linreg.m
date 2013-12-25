function [k,d,err]=mylinreg(t,f)

n=max(size(t));
sumt=sum(t);
k = (n*sum(f.*t)-sum(f)*sumt)/(n*sum(t.*t)-sumt*sumt);
d = sum(f-k*t)/n;


err=mean((k*t+d-f).^2);
