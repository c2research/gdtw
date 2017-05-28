function d=dtw_mink(s,t, ~)
% s: signal 1, size is ns*k, row for time, column for channel 
% t: signal 2, size is nt*k, row for time, column for channel 
% d: resulting distance

ns=length(s);
nt=length(t);

%% initialization
D=zeros(ns,nt); % cache matrix
D(1,1)=abs(s(:,1)-t(:,1));%norm(s(1,:)-t(1,:));
for i=1:ns-1
    D(i+1,1)=max(D(i,1),abs(s(:,i+1)-t(:,1)));
end
for j=1:nt-1
    D(1,j+1)=max(D(1,j),abs(s(:,1)-t(:,j+1)));
end

%% begin dynamic programming

for i=2:ns
    for j=2:nt      
        oost1=max(D(i-1,j), abs(s(:,i)-t(:,j)));
        oost2=max(D(i-1,j-1), abs(s(:,i)-t(:,j)));
        oost3=max(D(i,j-1), abs(s(:,i)-t(:,j)));
        %oost=norm(s(i,:)-t(j,:),1);
        oost=min(oost1,oost2);
        D(i,j)=min(oost,oost3);
        %D(i+1,j+1)=oost+min( [D(i,j+1), D(i+1,j), D(i,j)] );
        
    end
end

d=D(ns,nt);
