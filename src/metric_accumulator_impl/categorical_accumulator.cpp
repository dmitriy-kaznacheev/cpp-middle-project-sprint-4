#include "metric_accumulator_impl/categorical_accumulator.hpp"

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

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(
    const metric::MetricResult &metric_result) {
  if (is_finalized) {
    Reset();
  }

  auto value = std::to_string(metric_result.value);
  ++categories_freq[value];
}

void CategoricalAccumulator::Finalize() { is_finalized = true; }

void CategoricalAccumulator::Reset() {
  categories_freq.clear();
  is_finalized = false;
}

const std::unordered_map<std::string, int> &
CategoricalAccumulator::Get() const {
  if (!is_finalized) {
    throw std::runtime_error{"aggregation is not finalized"};
  }
  return categories_freq;
}

} // namespace analyser::metric_accumulator::metric_accumulator_impl
