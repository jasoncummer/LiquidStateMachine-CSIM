function [P,yapp,cc]=funfit(fun,x,y,P0,DISP)
%P=funfit(fun,x,y,P0,DISP) tries to fit the parameters of the function 'fun'
%  such that the summed squared error over the data 'x' and 'y' is
%  minimized (uses the function 'fminunc' provided with the optim toolbox).
%  'fun' should be given as a string and each parameter to fit
%  is identified with the two characters '%g'. If 'DISP' is '1'
%  plot is shown and some info at the terminal is printed (if ommited
%  '0' is assumed).
% 
%  Example if you want to fit a function like '(A+B*exp(-x/tau))' to the
%  data you would write P=fitfun('%g + %g * exp(-x ./ %g)',x,y). Then
%  'P(1)' corresponds to 'A', 'P(2)' corresponds to 'B' and 'P(3)' to
%  'tau'. In general 'P(i)' corresponds to the i-th occurrence of
%  '%g'.
%
%  'yapp(i)' is the value of 'fun' evaluated at 'x(i)' after the parameter
%  search. 'cc' is the correlation coefficient of 'yapp' and 'y'.

if nargin < 4, P0 = []; end
if nargin < 5, DISP = 0; end

%
% Make column vectors
%
x=x(:);
y=y(:);

%
% Replace all "%g" in fun with 'P(p)' where 'p' increses from
% to 'nParams'. The result is 'FUN'.
%
FITFUN = [];
nParams=0;
pos=findstr(fun,'%g');
for i=1:size(pos,2);
  nParams=nParams+1;
  if i==1
    FITFUN = [FITFUN fun(1:pos(i)-1) sprintf('P(%i)',nParams)];
  else
    FITFUN = [FITFUN fun((pos(i-1)+2):(pos(i)-1)) sprintf('P(%i)',nParams)];
  end
end
FITFUN = [FITFUN fun((pos(end)+2):end)];

%
% make inline function from FITFUN
%
fitfcn = inline(FITFUN,'P','x');

%
% The error function is defined as the summed squared error.
%
ERRFUN = sprintf('sum((y-(%s)).^2)',FITFUN);
errfcn = inline(ERRFUN,'P','x','y');

%
% Random guess of initial values for the parameters.
%

if isempty(P0)
  P0=rand(1,nParams);
  nIt=20;
else
  nIt=1;
end

ii=1;
cc=0;
while ( ii <= nIt & cc < 0.9 )
  if nIt > 1
    P0=rand(1,nParams);
  end
  %
  % define options for 'fminu'. Type 'help foptions' for more
  % information.
  %
  options=optimset('LargeScale','off','Diagnostics','off','Display','off');
  
  %
  % Run 'fminu'. Comes with the MatLab optim toolbox.
  %
  P=fminunc(errfcn,P0,options,x,y);
  
  yapp=fitfcn(P,x);
  cc=corrcoef(y,yapp);
  cc=cc(1,2);
  
  if ( cc < 0.9 )
    warning('very low correlation coefficient');
  end
  %
  % Display the resultin function in a plot and on the terminal.
  %
  if DISP
    xplot=min(x):(max(x)-min(x))/500:max(x);
    yplot=fitfcn(P,xplot);
    
    fmt=['\ny = ' fun '\n'];
    fprintf(fmt,P);
    err=errfcn(P,x,y);
    fprintf('mse = %g, correlation coefficient = %g\n\n',err,cc);
    
    figure(gcf); clf reset;
    plot(xplot,yplot,'k',x,y,'r*');
    axis tight
    title(sprintf('function fit: mse=%g, cc=%g',err,cc));
    xlabel('x');
    ylabel('f(x)');
    legend(sprintf(fun,P),'data');
    drawnow; pause(0.1);
  end
  ii=ii+1;
end
