function d=dtw_euclidean(s,t, ~)
% s: signal 1, size is ns*k, row for time, column for channel 
% t: signal 2, size is nt*k, row for time, column for channel 
% d: resulting distance
%

ns=length(s);
nt=length(t);
d=0;
%% initialization
D=zeros(ns,nt); % cache matrix
D(1,1)=(s(:,1)-t(:,1)).^2;%norm(s(1,:)-t(1,:));
for i=1:ns-1
    D(i+1,1)=D(i,1)+((s(:,i+1)-t(:,1)).^2);
end
for j=1:nt-1
    D(1,j+1)=D(1,j)+((s(:,1)-t(:,j+1)).^2);
end

% begin dynamic programming
for i=2:ns
    for j=2:nt      
        oost=(s(:,i)-t(:,j)).^2;
        D(i,j)=oost+min( [D(i-1,j), D(i,j-1), D(i-1,j-1)] );
        
    end
end

d=D(ns,nt);
