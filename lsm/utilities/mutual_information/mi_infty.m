function [resMIinfty, resMImodel, resCC] = mi_infty(argN,argMI,optOrder)
% MI_INFTY extrapolate mutual information to infinite data size
% 
%  Syntax
%
%    [MIinfty,MImodel,CC] = MI_INFTY(N,MIraw,order)
%
%  Arguments
%
%    MIraw   ... vector of raw/direkt MI measurements
%        N   ... vector of sample sizes; N(i) is the number of samples
%                from which MIraw(i) was directly estimated
%      order ... optional order of model to fit (see Algorithm)
% 
%    MIinfty ... extrapolation of mutual information to infinite sample size
%    MImodel ... mutual information predicted by model
%         CC ... correlation coefficient of the extrapolation
%
%  Description
%
%    MIinfty = MI_INFTY(N,MIraw) calculates the mutual
%    information MIinfty which one would expect to see for an infinite
%    number of samples given that for N(i) samples the direkt
%    measurement MIraw(i) (e.g. with mibayes) was obtained.
%
%    [MIinfty,MImodel,CC] = MI_INFTY(N,MIraw,order) allows you to
%    specify the order of the model which is used for extrapolation
%    (see Algorithm) and returns the correlation coefficient CC of the
%    fit of the model as well as the model output (see below).
%
%  Algorithm
%  
%    The parameters I0, I1, ..., Iorder of a model of the form 
%
%      Imodel = I0 + I1 * N^-1 + I2 * N^-2 + ... + Iorder * N^-order
%
%    are fitted with a linear regression to the data MI and N. I0 is
%    then considered as the infinite limit mututal information
%    MIinfty. See Strong et al 1998, Physical Review Letters,
%    80(1):197-200 for details. CC is the correlation coefficient
%    between the model-output Imodel and MIraw.
%
%  See also MIBAYES, MI_FROM_COUNT.
%
%  Author
%
%    Thomas Natschlaeger, Feb. 2002, tnatschl@igi.tu-graz.ac.at

% $Author: tnatschl $, $Revison$, $Date: 2003/05/26 12:42:24 $
% $Cross-Reference$

  if nargin < 3, optOrder = []; end
  if isempty(optOrder) optOrder = 1; end
  
  order = optOrder;
  n     = argN(:);
  mi    = argMI(:);

  if length(n) > 2
    powersOfN = repmat(n,[1 order+1]).^repmat([0:-1:-order],[length(n) 1]);
    
% do linear regression: result: b
    [Q, R]=qr(powersOfN,0);
    b = R\(Q'*mi);
    
    resMIinfty = b(1);
    resMImodel = powersOfN*b;
    resCC=corr_coef(mi,resMImodel);
  else
    resMIinfty = NaN;
    resMImodel = NaN;
    resCC=NaN;
  end 
