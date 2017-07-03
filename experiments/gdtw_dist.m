function [ D2 ] = gdtw_dist( ZI, ZJ, dist )
%GDTW_DIST Wrapper for c++ mex function gdtw 
%   Follows requirements for matlab distance functions
%   that can operate on 2-dimensional arrays
    [m2, ~] = size(ZJ);
    results = zeros(m2,1);
    parfor i=1:m2
       results(i) = gdtw(ZI, ZJ(i,:), dist); 
    end
    D2 = results;
end

