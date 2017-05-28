function dist = calculate_distance_manhatan(TimeSeries1,TimeSeries2,window_size)

%     dist = 0;
r = window_size;
%%%%%%%%%%%%%%
n = length(TimeSeries1);
m = length(TimeSeries2);

if (~exist('r','var')), r=0.1;              end
if (r<1),               r=ceil(min(n,m)*r); end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% compute the DTW distance between two time series
% to be replaced with .mex function compiled from C code
d = dtw_manhatan(TimeSeries1',TimeSeries2',r);

dist = d;


