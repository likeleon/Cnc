#include "cnc/stdafx.h"
#include "cnc/cursor_provider.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"

namespace cnc {

CursorProvider::CursorProvider(const ModData& mod_data)
  : mod_data_(mod_data) {
  auto sequence_files = mod_data_.manifest().cursors();
  auto yy = MiniYaml::FromFiles(sequence_files);
  auto accumulator = [](const auto& a, const auto& b) { return MiniYaml::MergePartial(a, b); };
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