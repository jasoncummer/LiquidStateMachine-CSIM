function s=vec2str(v,sep)
if nargin<2, sep=','; end
s=sprintf(sprintf('%s%%i',sep),v);
s=s((length(sep)+1):end);
return
