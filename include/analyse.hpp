#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto AnalyseFunctions(
    const std::vector<std::string> &files,
    const analyser::metric::MetricExtractor &metric_extractor) {
  // clang-format off
  return files | 
         std::views::transform([](auto &&fn) {
           auto f = file::File{fn};
           return function::FunctionExtractor{}.Get(f);
         }) |
         std::views::join |
         std::views::transform([&metric_extractor](auto &&func) {
           auto metrics = metric_extractor.Get(func);
           return std::make_pair(func, metrics);
         }) |
         rs::to<std::vector<std::pair<function::Function, metric::MetricResults>>>();
  // clang-format on
}

auto SplitByClasses(const auto &analysis) {
  // clang-format off
  return analysis | 
         std::views::filter([](const auto &p) {
           const auto &func = p.first;
           return func.class_name.has_value();
         }) |
         std::views::chunk_by([](const auto &lhs, const auto &rhs) {
           const auto &lfunc = lhs.first;
           const auto &rfunc = rhs.first; 
           return lfunc.class_name.value() == rfunc.class_name.value();
         }) |
         rs::to<std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>>>();
  // clang-format on
}

auto SplitByFiles(const auto &analysis) {
  // clang-format off
  return analysis | 
         std::views::chunk_by([](const auto &lhs, const auto &rhs) {
           return lhs.first.filename == rhs.first.filename;
         }) |
         rs::to<std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>>>();
  // clang-format on
}

void AccumulateFunctionAnalysis(
    const auto &analysis,
    const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
  // clang-format off
  rs::for_each(analysis,
               [&accumulator](const auto &metric) {
                 accumulator.AccumulateNextFunctionResults(metric);
               },
               &std::pair<function::Function, metric::MetricResults>::second);
  // clang-format on
}

} // namespace analyser
