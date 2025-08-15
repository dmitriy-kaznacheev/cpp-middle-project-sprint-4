#include "metric_accumulator.hpp"

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

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    rs::for_each(metric_results, [this](const auto &res) { accumulators.at(res.metric_name)->Accumulate(res); });
}

void MetricsAccumulator::ResetAccumulators() {
    rs::for_each(accumulators | std::views::values, [](auto &&acc) { acc->Reset(); });
}

}  // namespace analyser::metric_accumulator
