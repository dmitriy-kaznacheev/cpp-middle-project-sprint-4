#include "metric_impl/naming_style.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class naming_style
    : public ::testing::TestWithParam<std::tuple<std::string, std::string>> {
protected:
  NamingStyleMetric metric{};

  function::Function get_function(const std::string &function_name) {
    function::Function f{};
    f.name = function_name;
    return f;
  }
};

TEST_F(naming_style, name) { EXPECT_EQ(metric.Name(), "naming_style"s); }

TEST_P(naming_style, function) {
  auto expected = std::get<1>(GetParam());
  auto func_name = std::get<0>(GetParam());
  auto f = get_function(func_name);
  ASSERT_EQ(expected, std::get<std::string>(metric.CalculateImpl(f)));
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    , naming_style,
    ::testing::Values(
        std::make_tuple("functionname"s, "lower case"s),
        std::make_tuple("function_name"s, "snake case"s),
        std::make_tuple("functionName"s, "camel case"s),
        std::make_tuple("FunctionName"s, "pascal case"s),
        std::make_tuple("FUNCTION_NAME"s, "unknown"s)));
// clang-format off

#if 0
std::make_tuple("functionname"s, "lower case"s),
std::make_tuple("function_name"s, "snake case"s),
std::make_tuple("functionName"s, "camel case"s),
std::make_tuple("FunctionName"s, "pascal case"s),
std::make_tuple("FUNCTION_NAME"s, "unknown"s)
  const auto actual = std::get<std::string>(metric.CalculateImpl(f));

  EXPECT_EQ(expected, actual);
}
#endif

} // namespace analyser::metric::metric_impl
