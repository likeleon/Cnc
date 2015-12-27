#include "cnc/stdafx.h"
#include "cnc/chrome_metrics.h"
#include "cnc/mini_yaml.h"

namespace cnc {

std::map<std::string, std::string> ChromeMetrics::data_;

void ChromeMetrics::Initialize(const std::vector<std::string>& yaml) {
  std::vector<MiniYamlNodes> yy;
  for (const auto& y : yaml) {
    yy.emplace_back(*MiniYaml::FromFile(y));
  }
  MiniYamlNodes (*accumulator)(const MiniYamlNodes&, const MiniYamlNodes&) = &MiniYaml::MergePartial;
  auto partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);

  auto metrics = MiniYaml::ApplyRemovals(partial);
  for (const auto& m : metrics) {
    for (const auto& n : m.value().nodes()) {
      data_.emplace(n.key(), n.value().value());
    }
  }
}

}
