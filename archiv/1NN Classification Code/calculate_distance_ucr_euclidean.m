function dist = calculate_distance_ucr_euclidean(TimeSeries1,TimeSeries2,window_size)

% compute DTW distance between two time series
% window_size is a fraction of time series length. Its values go from 0-1

r = window_size;
n = length(TimeSeries1);
m = length(TimeSeries2);

%     if (~exist('r','var')), r=0.1;              end
if (r<=1)
    r=ceil(min(n,m)*r);
else
    disp('Window size value must be 0 to 1');
end

% compute the DTW distance between two time series
d = DTW_mex(TimeSeries1,TimeSeries2,r);

dist = d;

end
