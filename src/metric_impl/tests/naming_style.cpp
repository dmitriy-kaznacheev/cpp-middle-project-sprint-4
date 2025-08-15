#include "metric_impl/naming_style.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class naming_style : public testing::Test {
protected:
  NamingStyleMetric metric{};

  function::Function get_function(const std::string &function_name) {
    function::Function f{};
    f.name = function_name;
    return f;
  }
};

TEST_F(naming_style, name) { EXPECT_EQ(metric.Name(), "naming_style"s); }

TEST_F(naming_style, lower_case) {
  auto f = get_function("functionname"s);

  const auto expected = "lower case"s;
  const auto actual = std::get<std::string>(metric.CalculateImpl(f));

  EXPECT_EQ(expected, actual);
}

TEST_F(naming_style, snake_case) {
  auto f = get_function("function_name"s);

  const auto expected = "snake case"s;
  const auto actual = std::get<std::string>(metric.CalculateImpl(f));

  EXPECT_EQ(expected, actual);
}

TEST_F(naming_style, camel_case) {
  auto f = get_function("functionName"s);

  const auto expected = "camel case"s;
  const auto actual = std::get<std::string>(metric.CalculateImpl(f));

  EXPECT_EQ(expected, actual);
}

TEST_F(naming_style, pascal_case) {
  auto f = get_function("FunctionName"s);

  const auto expected = "pascal case"s;
  const auto actual = std::get<std::string>(metric.CalculateImpl(f));

  EXPECT_EQ(expected, actual);
}

TEST_F(naming_style, unknown) {
  auto f = get_function("FUNCTION_NAME"s);

  const auto expected = "unknown"s;
  const auto actual = std::get<std::string>(metric.CalculateImpl(f));

  EXPECT_EQ(expected, actual);
}

} // namespace analyser::metric::metric_impl
