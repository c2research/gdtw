#include <map>
#include <vector>
#include <chrono>

#include "Exception.hpp"

////////////////////////////////////////////////////////////////////////////////
/////**********      Include distance headers directly below     **********/////
////////////////////////////////////////////////////////////////////////////////
#include "distance/Euclidean.hpp"
#include "distance/Manhattan.hpp"
#include "distance/Chebyshev.hpp"
#include "distance/Cosine.hpp"
#include "distance/Distance.hpp"
#include "distance/Sorensen.hpp"

#include <ostream>

/**
 *  Add distances to this list
 */
static std::vector<dist_t> gAllDistance =
  {
    NEW_DISTANCE(Euclidean, data_t),
    NEW_DISTANCE(Manhattan, data_t),
    NEW_DISTANCE(Chebyshev, data_t),
    NEW_DISTANCE(Cosine, data_t*),
    NEW_DISTANCE(Sorensen, data_t*)
  };

/**
 *  Add name of distances to this list
 */
static std::vector<std::string> gAllDistanceName =
  {
    NEW_DISTANCE_NAME(euclidean),
    NEW_DISTANCE_NAME(manhattan),
    NEW_DISTANCE_NAME(chebyshev),
    NEW_DISTANCE_NAME(cosine),
    NEW_DISTANCE_NAME(sorensen)
  };

////////////////////////////////////////////////////////////////////////////////
/////**********          no need to make changes below!          **********/////
////////////////////////////////////////////////////////////////////////////////

bool gTraceDTWPath = false;
std::vector<std::vector<std::pair<int, int>>> gTrace;
std::string gLastDistance;

static std::map<std::string, dist_t> gAllDistanceMap;

void _initializeAllDistanceMap()
{
  if (gAllDistanceMap.empty())
  {
    for (unsigned int i = 0; i < gAllDistance.size(); i++)
    {
      gAllDistanceMap[gAllDistanceName[i]] = gAllDistance[i];
    }
  }
}

const dist_t getDistance(const std::string& distance_name)
{
  _initializeAllDistanceMap();
  if (gAllDistanceMap.find(distance_name) == gAllDistanceMap.end())
  {
    throw GenexException(std::string("Cannot find distance with name: ") + distance_name);
  }
  return gAllDistanceMap[distance_name];
}

const std::vector<std::string>& getAllDistanceName()
{
  return gAllDistanceName;
}

void printLastDTWPath(std::ostream& out) {
  if (gTrace.empty()) {
    throw GenexException("No DTW tracing data found");
  }

  std::vector<std::pair<int, int>> dtwPath;

  int i = gTrace.size() - 1, j = gTrace[0].size() - 1;
  while (i != TRACE_END_MARKER)
  {
    dtwPath.push_back(std::make_pair(i, j));
    int old_i = i, old_j = j;
    i = gTrace[old_i][old_j].first;
    j = gTrace[old_i][old_j].second;
  }
  auto now = std::chrono::system_clock::now();
  auto cnow = std::chrono::system_clock::to_time_t(now);
  out << "Computed at " << std::ctime(&cnow);
  out << "Used distance " << gLastDistance << std::endl;
  for (int i = dtwPath.size() - 1; i >= 0; i--) {
    out << dtwPath[i].first << " | " << dtwPath[i].second << std::endl;
  }
  out << "Warped count " << dtwPath.size() << std::endl;
  out << std::endl;
}
