#include "cnc/stdafx.h"
#include "cnc/cursor_provider.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"

namespace cnc {

CursorProvider::CursorProvider(const ModData& mod_data)
  : mod_data_(mod_data) {
  auto sequence_files = mod_data_.manifest().cursors();
  std::vector<MiniYamlNodes> yy;
  for (const auto& y : sequence_files) {
    yy.emplace_back(*MiniYaml::FromFile(y));
  }
  MiniYamlNodes(*accumulator)(const MiniYamlNodes&, const MiniYamlNodes&) = &MiniYaml::MergePartial;
  auto partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);

  MiniYaml sequences("", std::make_shared<MiniYamlNodes>(MiniYaml::ApplyRemovals(partial)));
  std::vector<int32_t> shadow_index;

  auto nodes_map = sequences.ToMap();
  if (nodes_map.find("ShadowIndex") != nodes_map.end()) {
    shadow_index.emplace_back(std::stoi(nodes_map.at("ShadowIndex").value()));
  }

  // TODO
}

}