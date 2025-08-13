#include "metric_impl/cyclomatic_complexity.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class cyclomatic_complexity : public testing::Test {
protected:
  CyclomaticComplexityMetric metric{};

  function::Function get_first_function(const std::string &filename) {
    file::File f{filename};
    function::FunctionExtractor extractor{};
    return extractor.Get(f).at(0);
  }
};

TEST_F(cyclomatic_complexity, name) {
  EXPECT_EQ(metric.Name(), "cyclomatic_complexity"s);
}

TEST_F(cyclomatic_complexity, comments) {
  auto f = get_first_function("comments.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 0);
}

TEST_F(cyclomatic_complexity, exceptions) {
  auto f = get_first_function("exceptions.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 4);
}

TEST_F(cyclomatic_complexity, if) {
  auto f = get_first_function("if.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(cyclomatic_complexity, loops) {
  auto f = get_first_function("loops.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 3);
}

TEST_F(cyclomatic_complexity, many_lines) {
  auto f = get_first_function("many_lines.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(cyclomatic_complexity, many_parameters) {
  auto f = get_first_function("many_parameters.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(cyclomatic_complexity, match_case) {
  auto f = get_first_function("match_case.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 4);
}

TEST_F(cyclomatic_complexity, nested_if) {
  auto f = get_first_function("nested_if.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 5);
}

TEST_F(cyclomatic_complexity, simple) {
  auto f = get_first_function("simple.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 1);
}

TEST_F(cyclomatic_complexity, ternary) {
  auto f = get_first_function("ternary.py"s);
  EXPECT_EQ(metric.CalculateImpl(f), 2);
}

} // namespace analyser::metric::metric_impl
