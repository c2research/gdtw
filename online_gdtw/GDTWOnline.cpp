#include "GDTWOnline.hpp"

#include "Exception.hpp"
#include "TimeSeriesSet.hpp"
#include "distance/Distance.hpp"

#include <fstream>
#include <string>

GDTWOnline::~GDTWOnline()
{
  unloadAllDataset();
}

dataset_info_t GDTWOnline::loadDataset(const std::string& filePath, int maxNumRow,
                                     int startCol, const std::string& separators)
{

  TimeSeriesSet* newSet = new TimeSeriesSet();
  try {
    newSet->loadData(filePath, maxNumRow, startCol, separators);
  } catch (GenexException& e)
  {
    delete newSet;
    throw e;
  }

  int nextIndex = -1;
  for (unsigned int i = 0; i < this->loadedDatasets.size(); i++)
  {
    if (this->loadedDatasets[i] == nullptr)
    {
      nextIndex = i;
      break;
    }
  }

  if (nextIndex < 0) {
    nextIndex = this->loadedDatasets.size();
    this->loadedDatasets.push_back(nullptr);
  }

  this->loadedDatasets[nextIndex] = newSet;
  this->datasetCount++;

  return this->getDatasetInfo(nextIndex);
}

void GDTWOnline::unloadDataset(int index)
{
  this->_checkDatasetIndex(index);

  delete loadedDatasets[index];
  loadedDatasets[index] = nullptr;
  if (index == loadedDatasets.size() - 1)
  {
    loadedDatasets.pop_back();
  }
  this->datasetCount--;
}

void GDTWOnline::unloadAllDataset()
{
  for (unsigned int i = 0; i < this->loadedDatasets.size(); i++)
  {
    delete this->loadedDatasets[i];
  }
  this->loadedDatasets.clear();
  this->datasetCount = 0;
}

int GDTWOnline::getDatasetCount()
{
  return this->datasetCount;
}

dataset_info_t GDTWOnline::getDatasetInfo(int index)
{
  this->_checkDatasetIndex(index);

  TimeSeriesSet* dataset = this->loadedDatasets[index];
  return dataset_info_t(index,
                        dataset->getFilePath(),
                        dataset->getItemCount(),
                        dataset->getItemLength());
}

std::vector<dataset_info_t> GDTWOnline::getAllDatasetInfo()
{
  std::vector<dataset_info_t> info;
  for (unsigned int i = 0; i < this->loadedDatasets.size(); i++)
  {
    if (loadedDatasets[i] != nullptr)
    {
      info.push_back(getDatasetInfo(i));
    }
  }
  return info;
}

std::vector<distance_info_t> GDTWOnline::getAllDistanceInfo()
{
  const std::vector<std::string>& allDistanceName = getAllDistanceName();
  std::vector<distance_info_t> info;
  for (auto name : allDistanceName)
  {
    info.push_back(distance_info_t(name, ""));
  }
  return info;
}

data_t GDTWOnline::getDistanceBetween(int index1, int idx1, int start1, int length1,
                                      int index2, int idx2, int start2, int length2,
                                      const std::string& distance_name)
{
  this->_checkDatasetIndex(index1);
  this->_checkDatasetIndex(index2);
  TimeSeries ts1 = this->loadedDatasets[index1]->getTimeSeries(idx1, start1, start1 + length1);
  TimeSeries ts2 = this->loadedDatasets[index2]->getTimeSeries(idx2, start2, start2 + length2);

  const dist_t distance = getDistance(distance_name);

  gTraceDTWPath = this->dtwPathFile.length() > 0;
  data_t result = distance(ts1, ts2, INF);

  if (this->dtwPathFile.length() > 0 && distance_name.find("_dtw") != std::string::npos) {
    gLastDistance = distance_name;
    std::ofstream outFile (this->dtwPathFile, std::ofstream::app);
    printLastDTWPath(outFile);
    outFile.close();
    std::cout << "DTW path printed to " + this->dtwPathFile << std::endl;
  }

  return result;
}

/**
  *  Set a file where the DTW path will be print to. Nothing will print
  *  if set this path to an empty string.
  */
void GDTWOnline::setPrintDTWPathToFile(const std::string& path) {
  if (path.length() > 0) {
    std::ofstream outFile(path, std::ofstream::out);
    outFile.close();
  }
  this->dtwPathFile = path;
}

candidate_time_series_t GDTWOnline::getBestMatchRaw(
    int index1, int idx1, int start1, int length1,
    int index2, const std::string &distance_name)
{
  this->_checkDatasetIndex(index1);
  this->_checkDatasetIndex(index2);

  TimeSeries target = this->loadedDatasets[index1]
                          ->getTimeSeries(idx1, start1, start1 + length1);

  int numberTimeSeries = this->loadedDatasets[index2]->getItemCount();
  int timeSeriesLength = this->loadedDatasets[index2]->getItemLength();

  data_t bestDist = INF;
  data_t currentDist;

  TimeSeries bestTimeSeries = TimeSeries(0);
  TimeSeries currentTimeSeries = TimeSeries(0);

  const dist_t distance = getDistance(distance_name);
  
  // Disable DTW tracing to improve performance
  gTraceDTWPath = false;

  // iterate through every timeseries
  for (int idx = 0; idx < numberTimeSeries; idx++) {
    //skip same time series
    if (idx1 == idx && index1 == index2) {
      continue;
    }
    // iterate through every length of interval
    for (int intervalLength = 1; intervalLength <= timeSeriesLength;
         intervalLength++) {
      // iterate through all interval window lengths
      for (int start = 0; start < timeSeriesLength - intervalLength; 
            start++) {
        currentTimeSeries = 
            this->loadedDatasets[index2]
                  ->getTimeSeries(idx, start, start + intervalLength);
        currentDist = distance(target, currentTimeSeries, INF);
        if (currentDist < bestDist) {
          bestDist = currentDist;
          bestTimeSeries = currentTimeSeries;
        }
      }
    }
  }
  return candidate_time_series_t(bestTimeSeries, bestDist);
}

void GDTWOnline::_checkDatasetIndex(int index)
{
  if (index < 0 || index >= loadedDatasets.size() || loadedDatasets[index] == nullptr)
  {
    throw GenexException("There is no dataset with given index");
  }
}



