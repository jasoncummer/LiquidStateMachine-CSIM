function cc=corr_coef(x,y)

if prod(size(unique(x))) == 1 | prod(size(unique(y))) == 1
%  warning('all equal');
  cc=NaN;
else
  cc=corrcoef(x,y);
  cc=cc(1,2);
end
