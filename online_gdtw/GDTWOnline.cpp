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

void GDTWOnline::_checkDatasetIndex(int index)
{
  if (index < 0 || index >= loadedDatasets.size() || loadedDatasets[index] == nullptr)
  {
    throw GenexException("There is no dataset with given index");
  }
}

