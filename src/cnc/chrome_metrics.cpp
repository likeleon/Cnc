#include "cnc/stdafx.h"
#include "cnc/chrome_metrics.h"
#include "cnc/mini_yaml.h"

namespace cnc {

void ChromeMetrics::Initialize(const std::vector<std::string>& yaml) {
  std::vector<MiniYamlNodes> yy;
  for (const auto& y : yaml) {
    yy.emplace_back(*MiniYaml::FromFile(y));
  }
  MiniYamlNodes (*accumulator)(const MiniYamlNodes&, const MiniYamlNodes&) = &MiniYaml::MergePartial;
  MiniYamlNodes partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);

  auto metrics = MiniYaml::ApplyRemovals(partial);
  for (const auto& m : metrics) {
    for (const auto& n : m.value().nodes()) {
      data_.emplace(n.key(), n.value().value());
    }
  }
}

}
