function verbose(varargin)

global VERBOSE_LEVEL

s=dbstack;

while isnumeric(varargin{1})
  varargin = varargin(2:end);
end

if VERBOSE_LEVEL >= length(s)-2
  fprintf(varargin{:});
end
