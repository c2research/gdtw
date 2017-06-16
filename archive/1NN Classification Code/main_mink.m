function [] = main_mink()
fileID = fopen('Mink_distance.txt', 'w');
fprintf(fileID, 'Dataset name, Error rate \r\n');

% % Tiny data sets < 10KB sorted by size
% one_NN_mink('SonyAIBORobotSurface');
% one_NN_mink('SonyAIBORobotSurfaceII');
% 
% % Small data sets 10-100KB sorted by size
% one_NN_mink('ItalyPowerDemand');
% one_NN_mink('TwoLeadECG');
% one_NN_mink('MoteStrain');
% one_NN_mink('ECGFiveDays');
% one_NN_mink('CBF');
% one_NN_mink('FaceFour');
% one_NN_mink('DiatomSizeReduction');
% one_NN_mink('Gun_Point');
% one_NN_mink('Coffee');
% one_NN_mink('ArrowHead');
% one_NN_mink('Symbols');
% one_NN_mink('ToeSegmentation2');
% one_NN_mink('ECG200');
% one_NN_mink('ShapeletSim');
% one_NN_mink('DistalPhalanxOutlineAgeGroup');
% one_NN_mink('BirdChicken');
% one_NN_mink('DistalPhalanxTW');
% one_NN_mink('BeetleFly');
% one_NN_mink('ToeSegmentation1');
% one_NN_mink('MiddlePhalanxTW');
% one_NN_mink('MiddlePhalanxOutlineAgeGroup');
% one_NN_mink('Wine');
% 
% % Medium datasets 100-500KB, sorted by size
% 
% one_NN_mink('Beef');
% one_NN_mink('ProximalPhalanxTW');
% one_NN_mink('Plane');
% one_NN_mink('OliveOil'); 
% one_NN_mink('synthetic_control');
% one_NN_mink('Lighting7');
% one_NN_mink('DistalPhalanxOutlineCorrect');
% one_NN_mink('MiddlePhalanxOutlineCorrect');
% one_NN_mink('Meat');
% one_NN_mink('FacesUCR');
% one_NN_mink('Trace');
% one_NN_mink('Earthquakes');
% one_NN_mink('ProximalPhalanxOutlineAgeGroup');
% one_NN_mink('Herring');
% one_NN_mink('Lighting2');
% one_NN_mink('Car');
% one_NN_mink('MedicalImages');
% one_NN_mink('ProximalPhalanxOutlineCorrect');
% one_NN_mink('Ham');
% one_NN_mink('MALLAT');
% one_NN_mink('InsectWingbeatSound');
% one_NN_mink('CinC_ECG_torso');
% one_NN_mink('SwedishLeaf');
% one_NN_mink('WormsTwoClass');
% one_NN_mink('Worms');

% % Medium datasets 500KB-1MB, sorted by size
% one_NN_mink('Adiac');
% one_NN_mink('ECG5000');
% one_NN_mink('WordSynonyms');
% one_NN_mink('FaceAll');
% one_NN_mink('Computers');
% one_NN_mink('ChlorineConcentration');
% one_NN_mink('FISH');
% one_NN_mink('Strawberry');
% one_NN_mink('OSULeaf');
% one_NN_mink('Cricket_X');
% one_NN_mink('Cricket_Y');
% one_NN_mink('Cricket_Z');
% one_NN_mink('50words');
% one_NN_mink('SmallKitchenAppliances');
% one_NN_mink('Two_Patterns');
% one_NN_mink('yoga');
% one_NN_mink('LargeKitchenAppliances');
% 
% % Large datasets 1-2MB, sorted by size
% one_NN_mink('PhalangesOutlinesCorrect');
% one_NN_mink('ScreenType');
% one_NN_mink('wafer');
% one_NN_mink('RefrigerationDevices');
% one_NN_mink('Haptics');
% one_NN_mink('InlineSkate');
% one_NN_mink('Phoneme');
% 
% % Large datasets 2-3MB, sorted by size
% one_NN_mink('UWaveGestureLibrary_Y');
% one_NN_mink('uWaveGestureLibrary_X');
% one_NN_mink('uWaveGestureLibrary_Z');
% one_NN_mink('ShapesAll');

% DATASETS LEFT TO COMPUTE

% Large datasets 3-4MB, sorted by size
one_NN_mink('FordB');

% Large datasets 4-5MB, sorted by size
one_NN_mink('FordA');

% Large datasets 6-7MB, sorted by size
one_NN_mink('uWaveGestureLibrary_All');
one_NN_mink('ElectricDevices');

% Large datasets 7-8MB, sorted by size
one_NN_mink('HandOutlines');
one_NN_mink('StarLightCurves');

% Large datasets 9-10MB, sorted by size
one_NN_mink('NonInvasiveFatalECG_Thorax2');
one_NN_mink('NonInvasiveFatalECG_Thorax1');    
  
end