#include "metric.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
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

#include "function.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) {
  if (!metric) {
    throw std::runtime_error{"metric for registration is invalid"};
  }
  metrics.emplace_back(std::move(metric));
}

MetricResults MetricExtractor::Get(const function::Function &func) const {
  return metrics | std::views::transform([&func](auto &&metric) {
           return metric->Calculate(func);
         }) |
         std::ranges::to<std::vector>();
}

} // namespace analyser::metric
