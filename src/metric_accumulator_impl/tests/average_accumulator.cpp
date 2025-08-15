#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using namespace std::string_literals;

class average : public testing::Test {
protected:
    AverageAccumulator acc{};
};

TEST_F(average, without_params) {
    acc.Reset();
    acc.Finalize();

    const auto expected = 0.0;
    const auto actual = acc.Get();

    EXPECT_DOUBLE_EQ(expected, actual);
}

TEST_F(average, one_param) {
    acc.Reset();
    acc.Accumulate({.metric_name = "metric"s, .value = 1});
    acc.Finalize();

    const auto expected = 1.0;
    const auto actual = acc.Get();

    EXPECT_DOUBLE_EQ(expected, actual);
}

TEST_F(average, multiple_params) {
    acc.Reset();
    acc.Accumulate({.metric_name = "metric"s, .value = 1});
    acc.Accumulate({.metric_name = "metric"s, .value = 2});
    acc.Accumulate({.metric_name = "metric"s, .value = 3});
    acc.Accumulate({.metric_name = "metric"s, .value = 4});
    acc.Finalize();

    const auto expected = 2.5;
    const auto actual = acc.Get();

    EXPECT_DOUBLE_EQ(expected, actual);
}

TEST_F(average, reset) {
    acc.Accumulate({.metric_name = "metric"s, .value = 1});
    acc.Accumulate({.metric_name = "metric"s, .value = 2});
    acc.Accumulate({.metric_name = "metric"s, .value = 3});
    acc.Reset();

    acc.Accumulate({.metric_name = "metric"s, .value = 42});
    acc.Finalize();

    const auto expected = 42.0;
    const auto actual = acc.Get();

    EXPECT_DOUBLE_EQ(expected, actual);
}

TEST_F(average, finalize) {
    acc.Accumulate({.metric_name = "metric"s, .value = 1});
    acc.Accumulate({.metric_name = "metric"s, .value = 2});
    acc.Accumulate({.metric_name = "metric"s, .value = 3});
    acc.Finalize();

    acc.Accumulate({.metric_name = "metric"s, .value = 42});
    acc.Finalize();

    const auto expected = 42.0;
    const auto actual = acc.Get();

    EXPECT_DOUBLE_EQ(expected, actual);
}

TEST_F(average, not_finalized) {
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

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
