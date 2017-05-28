function d=dtw_manhatan(s,t, ~)
% s: signal 1, size is ns*k, row for time, column for channel 
% t: signal 2, size is nt*k, row for time, column for channel 
% d: resulting distance

%



ns=length(s);
nt=length(t);
%calculate just MD distance 
index=min(ns,nt);
d=0;
%for i=1:index
%    d=d+abs(s(i,:)-t(i,:));
%end

%% initialization
D=zeros(ns,nt); % cache matrix
D(1,1)=abs(s(:,1)-t(:,1));%norm(s(1,:)-t(1,:));
for i=1:ns-1
    D(i+1,1)=D(i,1)+abs(s(:,i+1)-t(:,1));
end
for j=1:nt-1
    D(1,j+1)=D(1,j)+abs(s(:,1)-t(:,j+1));
end

% begin dynamic programming
for i=2:ns
    for j=2:nt      
        oost=abs(s(:,i)-t(:,j));
        D(i,j)=oost+min( [D(i-1,j), D(i,j-1), D(i-1,j-1)] );
        
    end
end

d=D(ns,nt);
