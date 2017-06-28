function [ test_error_rate ] = NearestNeighbor( distance, dataset )
%NearestNeighbor classifies `dataset` using the `distance`
%   utilizes gdtw to perform dtw.
data_path = '../../UCR/';

train_data = load(strcat(data_path, dataset, '/', dataset, '_TRAIN'));
train_label = train_data(:,1);
train_data = train_data(:, 2:end);

test_data = load(strcat(data_path, dataset, '/', dataset, '_TEST'));
test_label = test_data(:,1);
test_data = test_data(:, 2:end);

rng(1);

mdl = fitcknn(train_data, train_label, 'Standardize', 0);
mdl.Distance = @(x,Z) gdtw_dist(x, Z, distance);
k = 1;
mdl.NumNeighbors = k;

predicted_label = predict(mdl,test_data);
test_error_rate = sum(test_label ~= predicted_label)/length(test_label);
end

