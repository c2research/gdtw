#include "GDTWOnline.hpp"

#include "Exception.hpp"
#include "TimeSeriesSet.hpp"
#include "distance/Distance.hpp"

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

  return distance(ts1, ts2, INF);
}

void GDTWOnline::_checkDatasetIndex(int index)
{
  if (index < 0 || index >= loadedDatasets.size() || loadedDatasets[index] == nullptr)
  {
    throw GenexException("There is no dataset with given index");
  }
}



