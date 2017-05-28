%The following code finds aglomerative clustering using the following
%distances. Input file must be delimitted by space
%filename refers to the input file containing the sequences
% Euclidean, Manhhattan, Chebyshev, DTW_ED, GDTW_MD, GDTW_Mink

% To use pointwise distance e.g euclidean uncomment the two lines
%Y=zeros(m,n);
%Y = pdist(A,'euclidean');

% To find for warped distance,e.g for warped chebyshev uncomment Y(i,j)=dtw_mink(a,b);
clear;clc;
rng default;  % For reproducibility
filename = 'seq1.txt';
delimiterIn = ' ';
[A,delimiterIn] = importdata(filename)
[m,n]=size(A);
Y=zeros(m,n);
%calculate the distance 

for i=1:m
    for j=1:m
        a=A(i,:);
        b=A(j,:);
        %Y(i,j)=dtw_manhatan(a,b);
        Y(i,j)=dtw_mink(a,b);
        %Y(i,j)=dtw_euclidean(a,b);
    end
end

%Y=zeros(m,n);
%Y = pdist(A,'chebychev');
%Y = pdist(A,'euclidean');
%Y = pdist(A,'cityblock');
%squareform(Y);
Z = linkage(Y,'average');
dendrogram(Z)