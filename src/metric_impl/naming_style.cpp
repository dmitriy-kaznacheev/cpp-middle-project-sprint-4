#include "metric_impl/naming_style.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

MetricResult::ValueType
NamingStyleMetric::CalculateImpl(const function::Function &f) const {
  if (f.name.empty()) {
    return "unknown"s;
  }

  unsigned char first_symbol = f.name[0];
  const bool is_first_uppercase = std::isupper(first_symbol);
  const bool is_first_lowercase = std::islower(first_symbol);
  const bool contains_underscore = f.name.contains('_');
  const bool contains_uppercase = std::ranges::any_of(
      f.name, [](unsigned char c) { return std::isupper(c); });
  const bool contains_lowercase = std::ranges::any_of(
      f.name, [](unsigned char c) { return std::islower(c); });


  if (!contains_lowercase) {
      return "unknown"s;
  }

  if (!contains_uppercase && !contains_underscore) {
    return "lower case"s;
  }

  if (!contains_uppercase && contains_underscore) {
    return "snake case"s;
  }

  if (contains_uppercase && is_first_lowercase) {
    return "camel case"s;
  }

  if (contains_uppercase && is_first_uppercase) {
    return "pascal case"s;
  }

  return "unknown"s;
}

std::string NamingStyleMetric::Name() const { return "naming_style"s; }

} // namespace analyser::metric::metric_impl
