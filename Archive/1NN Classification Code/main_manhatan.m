function [] = main_manhatan()
fileID = fopen('Manhatan_distance.txt', 'w');
fprintf(fileID, 'Dataset name, Error rate \r\n');

% % Tiny data sets < 10KB sorted by size
% one_NN_manhatan('SonyAIBORobotSurface');
% one_NN_manhatan('SonyAIBORobotSurfaceII');
% 
% % Small data sets 10-100KB sorted by size
% one_NN_manhatan('ItalyPowerDemand');
% one_NN_manhatan('TwoLeadECG');
% one_NN_manhatan('MoteStrain');
% one_NN_manhatan('ECGFiveDays');
% one_NN_manhatan('CBF');
% one_NN_manhatan('FaceFour');
% one_NN_manhatan('DiatomSizeReduction');
% one_NN_manhatan('Gun_Point');
% one_NN_manhatan('Coffee');
% one_NN_manhatan('ArrowHead');
% one_NN_manhatan('Symbols');
% one_NN_manhatan('ToeSegmentation2');
% one_NN_manhatan('ECG200');
% one_NN_manhatan('ShapeletSim');
% one_NN_manhatan('DistalPhalanxOutlineAgeGroup');
% one_NN_manhatan('BirdChicken');
% one_NN_manhatan('DistalPhalanxTW');
% one_NN_manhatan('BeetleFly');
% one_NN_manhatan('ToeSegmentation1');
% one_NN_manhatan('MiddlePhalanxTW');
% one_NN_manhatan('MiddlePhalanxOutlineAgeGroup');
% one_NN_manhatan('Wine');

% % Medium datasets 100-500KB, sorted by size
% 
% one_NN_manhatan('Beef');
% one_NN_manhatan('ProximalPhalanxTW');
% one_NN_manhatan('Plane');
% one_NN_manhatan('OliveOil'); % not available
% one_NN_manhatan('synthetic_control');
% one_NN_manhatan('Lighting7');
% one_NN_manhatan('DistalPhalanxOutlineCorrect');
% one_NN_manhatan('MiddlePhalanxOutlineCorrect');
% one_NN_manhatan('Meat');
% one_NN_manhatan('FacesUCR');
% one_NN_manhatan('Trace');
% one_NN_manhatan('Earthquakes');
% one_NN_manhatan('ProximalPhalanxOutlineAgeGroup');
% one_NN_manhatan('Herring');
% one_NN_manhatan('Lighting2');
% one_NN_manhatan('Car');
% one_NN_manhatan('MedicalImages');
% one_NN_manhatan('ProximalPhalanxOutlineCorrect');
% one_NN_manhatan('Ham');
% one_NN_manhatan('MALLAT');
% one_NN_manhatan('InsectWingbeatSound');
% one_NN_manhatan('CinC_ECG_torso');
% one_NN_manhatan('SwedishLeaf');
% one_NN_manhatan('WormsTwoClass');
% one_NN_manhatan('Worms');
% 
% % Medium datasets 500KB-1MB, sorted by size
% one_NN_manhatan('Adiac');
% one_NN_manhatan('ECG5000');
% one_NN_manhatan('WordSynonyms');
% one_NN_manhatan('FaceAll');
% one_NN_manhatan('Computers');
% one_NN_manhatan('ChlorineConcentration');
% one_NN_manhatan('FISH');
% one_NN_manhatan('Strawberry');
% one_NN_manhatan('OSULeaf');
% one_NN_manhatan('Cricket_X');
% one_NN_manhatan('Cricket_Y');
% one_NN_manhatan('Cricket_Z');
% one_NN_manhatan('50words');
% one_NN_manhatan('SmallKitchenAppliances');
% one_NN_manhatan('Two_Patterns');
% one_NN_manhatan('yoga');
% one_NN_manhatan('LargeKitchenAppliances');
% 
% % Large datasets 1-2MB, sorted by size
% one_NN_manhatan('PhalangesOutlinesCorrect');
% one_NN_manhatan('ScreenType');
% one_NN_manhatan('wafer');
% one_NN_manhatan('RefrigerationDevices');
% one_NN_manhatan('Haptics');
% one_NN_manhatan('InlineSkate');
% one_NN_manhatan('Phoneme');

% DATASETS LEFT TO COMPUTE

% Large datasets 2-3MB, sorted by size
one_NN_manhatan('UWaveGestureLibrary_Y');
one_NN_manhatan('uWaveGestureLibrary_X');
one_NN_manhatan('uWaveGestureLibrary_Z');
one_NN_manhatan('ShapesAll');

% Large datasets 3-4MB, sorted by size
one_NN_manhatan('FordB');

% Large datasets 4-5MB, sorted by size
one_NN_manhatan('FordA');

% Large datasets 6-7MB, sorted by size
one_NN_manhatan('uWaveGestureLibrary_All');
one_NN_manhatan('ElectricDevices');

% Large datasets 7-8MB, sorted by size
one_NN_manhatan('HandOutlines');
one_NN_manhatan('StarLightCurves');

% Large datasets 9-10MB, sorted by size
one_NN_manhatan('NonInvasiveFatalECG_Thorax2');
one_NN_manhatan('NonInvasiveFatalECG_Thorax1');
  
end