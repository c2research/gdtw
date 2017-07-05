datasets={};
run('definitions.m')
for i=1:length(distances)
    distance = distances{i};
    disp('Starting distance: ');
    disp(distance);
    for j=1:length(datasets)
        dataset = datasets{j};
        disp('Starting dataset: ');
        disp(dataset);
        tic
        test_error_rate = NearestNeighbor(distance, dataset);
        elapsed = toc;
        
        fileID = fopen('nn_results_test.txt', 'a');

        fprintf(fileID, [distance, ', ', dataset, ', ', sprintf('%.3f', test_error_rate), ', ', sprintf('%.1f', elapsed), '\r\n']);
        fprintf( '%s, %s, %.3f, %f, \n', distance, dataset, test_error_rate, elapsed);

        fclose(fileID);
        pause(1);
    end
end
