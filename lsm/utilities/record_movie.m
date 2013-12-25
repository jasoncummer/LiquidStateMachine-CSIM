function M = record_movie(response, frameSize, optFrameTimes, optFilter, optMakeColorBar, optMakeTimeIndex)
%  RECORD_MOVIE Play/Record a 2D movie of a given response (or stimulus)
%
%  Syntax
%
%     M = record_movie(R,frameSize,t,F,makeColorBar,makeTimeIndex)
%
%  Arguments
%
%                R - a struct array holding the circuit response 
%                    with NC channels
%        frameSize - [Nx, Ny] matrix determining the size of the movie; 
%                    Note that Nx * Ny = NC is required.
%                t - times at which to generate/draw frames
%                    (optional; default: 0:1e-3:Tmax)
%                F - struct array describing the filter to apply to
%                    the data; for details see below
%                    (optional; default { @spikes2count 5e-3 } )
%     makeColorBar - If true a colorbar which relates the colors to
%                    numerical values is drawn;
%                    (optional; default: false)
%    makeTimeIndex - If true a time index is included in the movie
%                    (optional; default: false)
%                M - recorded movie
%
%
%  Description
%
%    record_movie(R, frameSize) converts the given response at each
%    given frame time (via the given filter) into a 2D images of size
%    Nx x Ny. The images are displayed continuously to get the
%    impression of a movie.
%
%    M = record_movie(R, frameSize) also records the frames and stores
%    them in the movie M which can be played by the command movie.
% 
%    M = record_movie(R, frameSize, t, F) uses the specified frame
%    times t and the specified filter F. F has to be a cell array
%    where
%
%     - F{1} is a mfile name or function handle (like spikes2exp)
%     - F{2:end} are parameter of the filter
%
%  Examples
%
%    a) Display spike counts in bins of 5ms (every 10ms)
%
%       >> record_movie(R, [25 25], 0:0.01:1, { @spikes2count 0.005 });
%
%    b) Record a movie (each 5ms a frame) of low-pass filtered spikes
%       (tau=30ms) and do a playback
%
%       >> M = record_movie(R, [25 25], 0:0.005:1, { @spikes2exp 0.03 });
%       >> movie(M);
%
%  See also
%
%    movie, getframe, moviein, spikes2alpha, spikes2exp, spikes2count
%
%  Author
%
%    Thomas Natschlaeger, Nils Bertschinger, {tnatschl,nilsb}@igi.tu-graz.ac.at
  
  if nargin < 2
    error('Please specifiy response and frameSize\n');
  end
  
  if nargin < 3, optFrameTimes    = []; end
  if nargin < 4, optFilter        = []; end
  if nargin < 5, optMakeColorBar  = 0; end
  if nargin < 6, optMakeTimeIndex = 1; end
  
  if isempty(optFrameTimes)
    optFrameTimes = 0:0.01:response.Tsim;
  end
  
  if isempty(optFilter)
    optFilter= { @spikes2count, 0.005, 0.001 };
  end
  
  % now go through recorded data and generate movie
  nFrames = length(optFrameTimes);

  % generate date with given optFilter
  filter = { optFilter{:} 0.005 0.001 }; % just to avoid that some
                                         % options are missing
  if isfield(response,'spiketimes')
    data = feval(filter{1},response,optFrameTimes,filter{2:end});
  elseif isfield(response,'channel')
    data = feval(filter{1},response.channel,optFrameTimes,filter{2:end});
  end
  
  % find min and max values for proper color scaling
  min_data = min(data(:));
  max_data = max(data(:));
  if max_data == min_data
    max_data = min_data + 1;
  end

  % init some memory for the movie if we record it
  M = [];
  if ( nargout > 0 )
    M = moviein(nFrames);
  end
  
  % set figure properties
  % clf reset;
  cla reset;
  colormap(hot);
  set(gca,'Clim',[min_data max_data]);

  % go throug all frame times
  for f = 1:nFrames

    ih=image(reshape(data(f,:),frameSize));
    
    set(gca,'Clim',[min_data max_data]);
    set(ih,'CDataMapping','scaled');
    axis xy equal tight;
    
    if optMakeTimeIndex
      text(frameSize(2), 1, sprintf('t=%g ms',optFrameTimes(f)*1000),...
           'FontWeight','bold','Color','k','EraseMode','xor', ...
           'HorizontalAlignment','right','VerticalAlignment','bottom');
    end
    
    if optMakeColorBar
      h=colorbar;
    end
    
    if nargout > 0
      M(:,f) = getframe;
    else
      drawnow;
    end
  end
    
