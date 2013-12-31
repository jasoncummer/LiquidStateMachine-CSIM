function MIFano = mi_fano(hx,pe,nx)
% MI_FANO Calculate the Fano lower bound on mutual information
% 
%  Syntax
%
%    MIfano = mi_fano(HX,Pe,NX)
%
%  Arguments
%
%      HX - entropy of the random variable X as returned by mibayes
%      Pe - probability of error; e.g. test or loo error
%      NX - number of different observations; e.g. length(tables.NX)
%
%      MIfano - Fano's lower bound on the mutual information
%
%  Description
%
%    MIfano = mi_fano(HX,Pe,NX) calculates a lower bound on the
%    mutual information by the Fano inequality (see Cover, 1991
%    pp. 38) where as the probability of error Perr the leave-one-out
%    estimate is used:
%
%      MIfano = HX - (H(Pe)+ Pe * ld(NX-1))
%
%    where H(Pe) = -Pe*ld(Pe)-(1-Pe)*ld(1-Pe)) and NX is
%    the number of different observations of the rv. X.
%
%  See also MIBAYES, MI_FROM_COUNT, MI_INFTY
%
%  Author
%
%    Thomas Natschlaeger, Feb. 2002, tnatschl@igi.tu-graz.ac.at

% $Author: tnatschl $, $Revision: 1.3 $, $Date: 2003/08/22 09:35:14 $
% $Cross-Reference$

  if pe == 1
    MIFano = 0;
  elseif pe == 0
    MIFano = hx(end);
  else
    MIFano = hx(end)-((-pe*log(pe)-(1-pe)*log(1-pe))+pe*log(nx-1))/log(2);
  end
  
