function print_struct(p,b)

if nargin<2,b=''; end

an=inputname(1);
fn=fieldnames(p);
[m,n]=size(p);
for i=1:m
  for j=1:n
    if m>1 & n > 1
      lb=sprintf('%s%s(%i,%i).',b,an,i,j);
    elseif m > 1 | n > 1
      lb=sprintf('%s%s(%i).',b,an,(i-1)*n+j);
    else
       lb=sprintf('%s%s.',b,an);
     end  
    for f=1:length(fn)
      eval(sprintf('x=p(i,j).%s;',fn{f}));
      if isempty(x)
	% fprintf('%s%s = []\n',lb,fn{f});
      elseif isnumeric(x)
	if length(x(:))>10
	  fprintf('%s%s = [%ix%i double]\n',lb,fn{f},size(x,1),size(x,2));
	elseif length(x(:))>1
	  fprintf('%s%s = [ %g%s ]\n',lb,fn{f},x(1),sprintf(' %g',x(2:end)));
	else
	  fprintf('%s%s = %g\n',lb,fn{f},x);
	end
      elseif isstruct(x)
	eval(sprintf('%s=p(i,j).%s;',fn{f},fn{f}));
	l=length(fn{f});
	eval(sprintf('print_struct(%s,lb)',fn{f}));
      end
    end
  end
end