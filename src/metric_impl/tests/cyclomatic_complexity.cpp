#include "metric_impl/cyclomatic_complexity.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class cyclomatic_complexity
    : public ::testing::TestWithParam<std::tuple<std::string, int>> {
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

TEST_P(cyclomatic_complexity, file) {
  auto expected = std::get<1>(GetParam());
  auto filename = std::get<0>(GetParam());
  auto f = get_first_function(filename);
  ASSERT_EQ(expected, std::get<int>(metric.CalculateImpl(f)));
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    , cyclomatic_complexity,
    ::testing::Values(
        std::make_tuple("comments.py"s, 0),
        std::make_tuple("exceptions.py"s, 4), 
        std::make_tuple("if.py"s, 1),
        std::make_tuple("loops.py"s, 3), 
        std::make_tuple("many_lines.py"s, 1),
        std::make_tuple("many_parameters.py"s, 1),
        std::make_tuple("match_case.py"s, 4),
        std::make_tuple("nested_if.py"s, 5), 
        std::make_tuple("simple.py"s, 1),
        std::make_tuple("ternary.py"s, 2)));
// clang-format off

} // namespace analyser::metric::metric_impl
