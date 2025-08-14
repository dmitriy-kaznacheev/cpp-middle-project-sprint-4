#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class parameters_count : public testing::Test {
protected:
  CountParametersMetric metric{};

  function::Function get_first_function(const std::string &filename) {
    file::File f{filename};
    function::FunctionExtractor extractor{};
    return extractor.Get(f).at(0);
  }
};

TEST_F(parameters_count, name) {
  EXPECT_EQ(metric.Name(), "parameters_count"s);
}

TEST_F(parameters_count, comments) {
  auto f = get_first_function("comments.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 3);
}

TEST_F(parameters_count, exceptions) {
  auto f = get_first_function("exceptions.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 0);
}

TEST_F(parameters_count, if) {
  auto f = get_first_function("if.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(parameters_count, loops) {
  auto f = get_first_function("loops.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(parameters_count, many_lines) {
  auto f = get_first_function("many_lines.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 0);
}

TEST_F(parameters_count, many_parameters) {
  auto f = get_first_function("many_parameters.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 5);
}

TEST_F(parameters_count, match_case) {
  auto f = get_first_function("match_case.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(parameters_count, nested_if) {
  auto f = get_first_function("nested_if.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 2);
}

TEST_F(parameters_count, simple) {
  auto f = get_first_function("simple.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 0);
}

TEST_F(parameters_count, ternary) {
  auto f = get_first_function("ternary.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

} // namespace analyser::metric::metric_impl
