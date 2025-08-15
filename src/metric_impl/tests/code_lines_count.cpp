#include "metric_impl/code_lines_count.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class lines_count : public ::testing::TestWithParam<std::tuple<std::string, int>> {
protected:
    CodeLinesCountMetric metric{};

    function::Function get_first_function(const std::string &filename) {
        file::File f{filename};
        function::FunctionExtractor extractor{};
        return extractor.Get(f).at(0);
    }
};

TEST_F(lines_count, name) { EXPECT_EQ(metric.Name(), "lines_count"s); }

TEST_P(lines_count, file) {
    auto expected = std::get<1>(GetParam());
    auto filename = std::get<0>(GetParam());
    auto f = get_first_function(filename);
    ASSERT_EQ(expected, std::get<int>(metric.CalculateImpl(f)));
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    , lines_count,
    ::testing::Values(
        std::make_tuple("comments.py"s, 4),
        std::make_tuple("exceptions.py"s, 8), 
        std::make_tuple("if.py"s, 4),
        std::make_tuple("loops.py"s, 7), 
        std::make_tuple("many_lines.py"s, 11),
        std::make_tuple("many_parameters.py"s, 2),
        std::make_tuple("match_case.py"s, 8),
        std::make_tuple("nested_if.py"s, 9), 
        std::make_tuple("simple.py"s, 6),
        std::make_tuple("ternary.py"s, 2)));
// clang-format off

} // namespace analyser::metric::metric_impl
