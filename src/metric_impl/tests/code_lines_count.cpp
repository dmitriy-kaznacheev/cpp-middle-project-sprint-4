#include "metric_impl/code_lines_count.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class lines_count : public testing::Test {
protected:
  CodeLinesCountMetric metric{};

  function::Function get_first_function(const std::string &filename) {
    file::File f{filename};
    function::FunctionExtractor extractor{};
    return extractor.Get(f).at(0);
  }
};

TEST_F(lines_count, name) { EXPECT_EQ(metric.Name(), "lines_count"s); }

TEST_F(lines_count, comments) {
  auto f = get_first_function("comments.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 4);
}

TEST_F(lines_count, exceptions) {
  auto f = get_first_function("exceptions.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 8);
}

TEST_F(lines_count, if) {
  auto f = get_first_function("if.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 4);
}

TEST_F(lines_count, loops) {
  auto f = get_first_function("loops.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 7);
}

TEST_F(lines_count, many_lines) {
  auto f = get_first_function("many_lines.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 11);
}

TEST_F(lines_count, many_parameters) {
  auto f = get_first_function("many_parameters.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 2);
}

TEST_F(lines_count, match_case) {
  auto f = get_first_function("match_case.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 8);
}

TEST_F(lines_count, nested_if) {
  auto f = get_first_function("nested_if.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 9);
}

TEST_F(lines_count, simple) {
  auto f = get_first_function("simple.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 6);
}

TEST_F(lines_count, ternary) {
  auto f = get_first_function("ternary.py"s);
  EXPECT_EQ(std::get<int>(metric.CalculateImpl(f)), 2);
}

} // namespace analyser::metric::metric_impl
