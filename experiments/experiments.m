for i=1:length(distances)
    dist = distances{i};
    
    disp('starting distance: ')
    disp(dist)
    
    for j=1:length(datasets)
        set = datasets{j};
        NearestNeighbor(dist, set);
        
        disp('finished dataset: ')
        disp(set)
    end
    
    disp('finished distance: ')
    disp(dist)
end