#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using SumAverage = SumAverageAccumulator::SumAverage;
using namespace std::string_literals;

class sum_average : public testing::Test {
protected:
  SumAverageAccumulator acc{};
};

TEST_F(sum_average, without_params) {
  acc.Reset();
  acc.Finalize();

  const auto expected = SumAverage{.sum = 0, .average = 0.0};
  const auto actual = acc.Get();

  EXPECT_EQ(expected, actual);
}

TEST_F(sum_average, one_param) {
  acc.Reset();
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Finalize();

  const auto expected = SumAverage{.sum = 1, .average = 1.0};
  const auto actual = acc.Get();

  EXPECT_EQ(expected, actual);
}

TEST_F(sum_average, multiple_params) {
  acc.Reset();
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 3});
  acc.Accumulate({.metric_name = "metric"s, .value = 4});
  acc.Finalize();

  const auto expected = SumAverage{.sum = 10, .average = 2.5};
  const auto actual = acc.Get();

  EXPECT_EQ(expected, actual);
}

TEST_F(sum_average, reset) {
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 3});
  acc.Reset();

  acc.Accumulate({.metric_name = "metric"s, .value = 42});
  acc.Finalize();

  const auto expected = SumAverage{.sum = 42, .average = 42.0};
  const auto actual = acc.Get();

  EXPECT_EQ(expected, actual);
}

TEST_F(sum_average, finalize) {
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 3});
  acc.Finalize();

  acc.Accumulate({.metric_name = "metric"s, .value = 42});
  acc.Finalize();

  const auto expected = SumAverage{.sum = 42, .average = 42.0};
  const auto actual = acc.Get();

  EXPECT_EQ(expected, actual);
}

TEST_F(sum_average, not_finalized) {
  acc.Reset();
  acc.Accumulate({.metric_name = "metric"s, .value = 42});

  EXPECT_THROW(
      {
        try {
          acc.Get();
        } catch (const std::runtime_error &e) {
          EXPECT_STREQ("aggregation is not finalized", e.what());
          throw;
        }
      },
      std::runtime_error);
}

} // namespace analyser::metric_accumulator::metric_accumulator_impl::test
