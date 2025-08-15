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

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

using namespace std::string_literals;

const std::array acc_names{
  "lines_count"s,
  "cyclomatic_complexity"s,
  "parameters_count"s,
  "naming_style"s,
};

int main(int argc, char *argv[]) {
  analyser::cmd::ProgramOptions options;
  if (!options.Parse(argc, argv)) {
    return EXIT_FAILURE;
  }

  try {
    // clang-format off
    analyser::metric::MetricExtractor metric_extractor;
    {
      using namespace analyser::metric::metric_impl;
      metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
      metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
      metric_extractor.RegisterMetric(std::make_unique<CountParametersMetric>());
      metric_extractor.RegisterMetric(std::make_unique<NamingStyleMetric>()); 
    }

    auto analysis = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);
    analysis | 
    std::views::transform([](auto &&p) {
      const auto &[func, metrics] = p;
      const auto class_name = func.class_name.has_value() ? ("::"s + func.class_name.value()) : ""s;
      std::println("{}{}::{}", func.filename, class_name, func.name);
      return metrics;
    }) | 
    std::views::join |
    std::views::transform([](const auto &metric) {
      std::print("    {}: ", metric.metric_name);
      std::visit([](const auto &v) { std::println("{}", v); }, metric.value);
      return 0;
    }) |
    rs::to<std::vector>();

    analyser::metric_accumulator::MetricsAccumulator accumulator;
    {
      using namespace analyser::metric_accumulator::metric_accumulator_impl;
      accumulator.RegisterAccumulator(acc_names[0], std::make_unique<SumAverageAccumulator>());
      accumulator.RegisterAccumulator(acc_names[1], std::make_unique<SumAverageAccumulator>());
      accumulator.RegisterAccumulator(acc_names[2], std::make_unique<AverageAccumulator>());
      accumulator.RegisterAccumulator(acc_names[3], std::make_unique<CategoricalAccumulator>()); 
    }

    auto by_files = analyser::SplitByFiles(analysis);
    rs::for_each(by_files, [&](const auto &grp_analysis) {
      if (grp_analysis.empty()) {
        return;
      }

      accumulator.ResetAccumulators();
      analyser::AccumulateFunctionAnalysis(grp_analysis, accumulator);
      const auto &filename = grp_analysis.front().first.filename;
      std::println("Accumulated Analysis for file {}:", filename);

      using namespace analyser::metric_accumulator::metric_accumulator_impl;

      {
        auto &acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(acc_names[0]);
        auto res = acc.Get();
        std::println("    {}: sum = {}, average = {:.2f}", acc_names[0], res.sum, res.average);
      }

      {
        auto &acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(acc_names[1]);
        auto res = acc.Get();
        std::println("    {}: sum = {}, average = {:.2f}", acc_names[1], res.sum, res.average);
      }

      { 
        auto &acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(acc_names[2]);
        auto res = acc.Get();
        std::println("    {}: average = {:.2f}", acc_names[2], res);
      }

      {
        auto &acc = accumulator.GetFinalizedAccumulator<CategoricalAccumulator>(acc_names[3]);
        std::println("    {}:", acc_names[3]);
        auto res = acc.Get();
        std::ranges::for_each(res, [](const auto &p) {
          const auto &[style, count] = p;
          std::println("        {}: {}", style, count);
        });
      }
    });

    auto by_classes = analyser::SplitByClasses(analysis);
    rs::for_each(by_classes, [&](const auto &grp_analysis) {
      if (grp_analysis.empty()) {
        return;
      }

      auto &[func, metrics] = grp_analysis.front();
      if (!func.class_name.has_value()) {
        return;
      }

      accumulator.ResetAccumulators();
      analyser::AccumulateFunctionAnalysis(grp_analysis, accumulator);
      std::println("Accumulated Analysis for class {}:", func.class_name.value());

      using namespace analyser::metric_accumulator::metric_accumulator_impl;

      {
        auto &acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(acc_names[0]);
        auto res = acc.Get();
        std::println("    {}: sum = {}, average = {:.2f}", acc_names[0], res.sum, res.average);
      }

      {
        auto &acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(acc_names[1]);
        auto res = acc.Get();
        std::println("    {}: sum = {}, average = {:.2f}", acc_names[1], res.sum, res.average);
      }      

      { 
        auto &acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(acc_names[2]);
        auto res = acc.Get();
        std::println("    {}: average = {:.2f}", acc_names[2], res);
      }

      {
        auto &acc = accumulator.GetFinalizedAccumulator<CategoricalAccumulator>(acc_names[3]);
        auto res = acc.Get();
        std::ranges::for_each(res, [](const auto &p) {
          const auto &[style, count] = p;
          std::println("        {}: {}", style, count);
        });
      }
    });
    // clang-format 
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
