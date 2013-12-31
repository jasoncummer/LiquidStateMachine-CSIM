function a = pm_available(varargin)
% PM_AVAILABLE returns 1 if there is a running parallel matlab;
%               zero otherwise.

% $Id: pm_available.m,v 1.1.1.1 2002/11/14 14:35:31 tnatschl Exp $
% $Author: tnatschl $
% $Date: 2002/11/14 14:35:31 $
% $Log: pm_available.m,v $
% Revision 1.1.1.1  2002/11/14 14:35:31  tnatschl
% New direcotry structure
%
% Revision 1.1.1.1  2002/10/03 15:22:27  tnatschl
% initial revision
%
%
% Revision 1.2  2002/06/10 13:41:02  tnatschl
% added check for pmis.
%

if isempty(getenv('PM_PATH'))
  a = 0;
else
  if exist('pmis') ~= 2
    a = 0;
  else
    a= pmis;
  end
end


