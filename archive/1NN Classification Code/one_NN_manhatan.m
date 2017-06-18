% 1NN classification
% Input
% train_data and test_data: each row is a time series ; first column is label
% w: warping window width in percentage. 1 is 1%
% k: number of nearest neighbours
% Output
% test error rate
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%%
function [test_error_rate] = one_NN_manhatan(dataset_name, w, k)
% close all; clc;

disp(dataset_name);

if ~exist('w', 'var')
    w = 100;
end

if ~exist('k', 'var')
    k = 1;
end

train_data = load(strcat('ucr_train/', dataset_name,'_train.mat'));
train_data = train_data.data;
train_label = train_data(:,1);
train_data = train_data(:, 2:end);
% train_data = zscore(train_data')';

test_data = load(strcat('ucr_test/', dataset_name,'_test.mat'));
test_data = test_data.data;
test_label = test_data(:,1);
test_data = test_data(:, 2:end);
% test_data = zscore(test_data')';

rng(1);

% disp(strcat(['Warping window width is ', num2str(w)]));

mdl = fitcknn(train_data, train_label, 'Standardize', 0);

mdl.Distance = @(x,Z)distfun_manhatan(x,Z, w/100);

mdl.NumNeighbors = k;

predicted_label = predict(mdl,test_data);
test_error_rate = sum(test_label ~= predicted_label)/length(test_label);

fileID = fopen('Manhatan_distance.txt', 'a');
fprintf(fileID, [dataset_name, ', ', sprintf('%.2f', test_error_rate), '\r\n']);

end

