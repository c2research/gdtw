#define BOOST_TEST_MODULE "Test TimeSeriesSet class"

#include <boost/test/unit_test.hpp>

#include "GDTWOnline.hpp"
#include "Exception.hpp"

struct MockDataset
{
  std::string test_10_5_space = "test/datasets/test_10_5_space.txt";
} data;

BOOST_AUTO_TEST_CASE( basic_online_get_raw_best_match )
{
  GDTWOnline online;
  online.loadDataset(data.test_10_5_space, 5, 0, " ");
  online.loadDataset(data.test_10_5_space, 5, 0, " ");

  std::string dist_name = "euclidean";
  candidate_time_series_t best_1 = online.getBestMatchRaw(0, 0, 0, 5, 0, dist_name);
  candidate_time_series_t best_2 = online.getBestMatchRaw(0, 0, 0, 5, 1, dist_name);
  candidate_time_series_t best_3 = online.getBestMatchRaw(0, 1, 0, 5, 0, dist_name);
  candidate_time_series_t best_4 = online.getBestMatchRaw(0, 1, 5, 5, 1, dist_name);

  BOOST_TEST(best_1.dist == 0.0);
  BOOST_TEST(best_2.dist == 0.0);
  BOOST_TEST(best_3.dist == 0.0);
  BOOST_TEST(best_4.dist == 0.0);

  BOOST_CHECK_THROW( online.getBestMatchRaw(3, 0, 0, 0, 0, dist_name), GenexException ); // dataset not loaded
  BOOST_CHECK_THROW( online.getBestMatchRaw(1, 35, 0, 0, 0, dist_name), GenexException ); // not that many ts in dataset
  BOOST_CHECK_THROW( online.getBestMatchRaw(1, 0, 100, 125, 0, dist_name), GenexException ); // not that big ts in dataset
}

