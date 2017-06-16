function [] = main_ucr_euclidean()
fileID = fopen('Euclidean_distance_UCR.txt', 'w');
fprintf(fileID, 'Dataset name, Error rate \r\n');

% Tiny data sets < 10KB sorted by size
one_NN_ucr_euclidean('SonyAIBORobotSurface');

end