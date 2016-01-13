#include "cnc/stdafx.h"
#include "cnc/chrome_metrics.h"
#include "cnc/mini_yaml.h"

namespace cnc {

std::map<std::string, std::string> ChromeMetrics::data_;

void ChromeMetrics::Initialize(const std::vector<std::string>& yaml) {
  auto yy = MiniYaml::FromFiles(yaml);
  auto accumulator = [](const auto& a, const auto& b) { return MiniYaml::MergePartial(a, b); };
  auto partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);

  auto metrics = MiniYaml::ApplyRemovals(partial);
  for (const auto& m : metrics) {
    for (const auto& n : m.value().nodes()) {
      data_.emplace(n.key(), n.value().value());
    }
  }
}

}
