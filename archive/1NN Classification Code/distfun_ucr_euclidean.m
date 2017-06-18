function d2 = distfun_ucr_euclidean(Xi, Xj, band)
    % how to do this I don't know hihi
    % Xi: a single row of x
    % Xj: multiple rows of X
    % d2: vector of distances whose k-th element is
    % the distance between Xi and Xj(k,:)
    
%     band = 0;
    
    rows_in_Xj = size(Xj, 1);
    D = zeros(rows_in_Xj, 1);
    for i = 1:rows_in_Xj
        ts1 = Xi;
        ts2 = Xj(i,:);
        D(i, 1) = calculate_distance_ucr_euclidean(ts1, ts2, band);
    end
    d2 = D;
    
    
    

