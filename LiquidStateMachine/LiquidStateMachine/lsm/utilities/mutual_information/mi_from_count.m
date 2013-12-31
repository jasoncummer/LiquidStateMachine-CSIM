function [MI,Hx,Hy,Hxy] = mi_from_count(argNXY)
% MI_FROM_COUNT compute mutual information from joint count table
% 
%   Syntax
%   
%     [MI,HX,HY,HXY] = mi_from_count(Nxy)
%     
%   Arguments
%    
%     Nxy ... joint count table; i.e. Nxy(i,j) is the number of occurrences
%             (or probabilty) of observation of pairs (i,j).
%             
%      MI ... mutual information
%      HX ... entropy of X
%      HY ... entropy of Y
%     HXY ... joint entropy
% 
%   Description
%   
%     [MI,HX,HY,HXY] = mi_from_count(Nxy) computes the mutual
%     information and the related entropies from the given joint cout
%     table (joint probability density function).
% 
%   Algorithm
%
%     MI = SUM_i SUM_j Nxy(i,j) * ld Nxy(i,j) / ( nx(i) * ny(j) )
%     nx(i) = SUM_j Nxy(i,j)
%     ny(j) = SUM_i Nxy(i,j)
%
%    See also
%    
%      mibayes, mi_infty
%  
% Author: Thomas Natschlaeger, Feb. 2002, tnatschl@igi.tu-graz.ac.at

% $Author: tnatschl $, $Revison$, $Date: 2003/05/26 12:42:24 $
% $Cross-Reference$


  if prod(size(argNXY)) == 0
    MI = NaN; Hx=NaN; Hy=NaN; Hxy=NaN;
    return;
  end
  
  pxy = argNXY/sum(argNXY(:));
  pxy(:,all(pxy==0,1)) = [];
  pxy(all(pxy==0,2),:) = [];
  
  px = sum(pxy,2);
  py = sum(pxy,1);
  
  Hx = -sum(px.*log(px))/log(2);
  
  Hy = -sum(py.*log(py))/log(2);

  p=pxy./(px*ones(1,size(pxy,2)));
  p(p==0)=1;
  H=-sum(p.*log(p),2)/log(2);
  Hyx = full(sum(px.*H));
  
  MI = Hy - Hyx;
  
  Hxy = Hx + Hyx;


