#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;
using namespace std::string_view_literals;

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    static const std::unordered_set keys = {"if_statement"sv,    "else_clause"sv,      "elif_clause"sv,
                                            "while_statement"sv, "for_statement"sv,    "try_statement"sv,
                                            "except_clause"sv,   "finally_clause"sv,   "match_statement"sv,
                                            "case_pattern"sv,    "assert_statement"sv, "conditional_expression"sv};

    auto res = std::ranges::distance(f.ast | std::views::split('(') | std::views::drop(1) |
                                     std::views::transform([](auto &&r) {
                                         auto sv = std::string_view{r};
                                         return sv.substr(0, sv.find_first_of(" :"));
                                     }) |
                                     std::views::filter([](auto &&sv) { return keys.contains(std::string{sv}); }));
    return static_cast<int>(res);
}

std::string CyclomaticComplexityMetric::Name() const { return "cyclomatic_complexity"s; }

}  // namespace analyser::metric::metric_impl
