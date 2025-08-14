#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using namespace std::string_literals;

class categorical : public testing::Test {
protected:
  CategoricalAccumulator acc{};
};

TEST_F(categorical, without_params) {
  acc.Reset();
  acc.Finalize();

  const auto categories = acc.Get();

  EXPECT_TRUE(categories.empty());
}

TEST_F(categorical, one_param) {
  acc.Reset();
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Finalize();

  const auto categories = acc.Get();

  EXPECT_EQ(categories.size(), 1);
  EXPECT_EQ(categories.at("1"s), 1);
}

TEST_F(categorical, multiple_params) {
  acc.Reset();
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Finalize();

  const auto categories = acc.Get();

  EXPECT_EQ(categories.size(), 2);
  EXPECT_EQ(categories.at("1"s), 1);
  EXPECT_EQ(categories.at("2"s), 3);
}

TEST_F(categorical, reset) {
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 3});
  acc.Reset();

  acc.Accumulate({.metric_name = "metric"s, .value = 42});
  acc.Finalize();

  const auto categories = acc.Get();

  EXPECT_EQ(categories.size(), 1);
  EXPECT_EQ(categories.at("42"s), 1);
}

TEST_F(categorical, finalize) {
  acc.Accumulate({.metric_name = "metric"s, .value = 1});
  acc.Accumulate({.metric_name = "metric"s, .value = 2});
  acc.Accumulate({.metric_name = "metric"s, .value = 3});
  acc.Finalize();

  acc.Accumulate({.metric_name = "metric"s, .value = 42});
  acc.Finalize();

  const auto categories = acc.Get();

  EXPECT_EQ(categories.size(), 1);
  EXPECT_EQ(categories.at("42"s), 1);
}

TEST_F(categorical, not_finalized) {
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
