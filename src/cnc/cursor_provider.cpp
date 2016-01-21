#include "cnc/stdafx.h"
#include "cnc/cursor_provider.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"
#include "cnc/game.h"
#include "cnc/settings.h"

namespace cnc {

CursorProvider::CursorProvider(ModData& mod_data)
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

  //TODO: palettes

  FrameCache frame_cache(mod_data_.sprite_loaders());
  for (const auto& s : nodes_map.at("Cursors").nodes()) {
    for (const auto& sequence : s.value().nodes()) {
      cursors_.emplace(std::piecewise_construct,
                       std::forward_as_tuple(sequence.key()), 
                       std::forward_as_tuple(frame_cache, sequence.key(), s.key(), s.value().value(), sequence.value()));
    }
  }
}

bool CursorProvider::CursorViewportZoomed() {
  const auto& g = Game::settings().graphics();
  return g.cursor_double && g.pixel_double;
}

bool CursorProvider::HasCursorSequence(const std::string& cursor) const {
  return cursors_.find(cursor) != cursors_.end();
}

const CursorSequence& CursorProvider::GetCursorSequence(const std::string& cursor) const {
  try {
    return cursors_.at(cursor);
  } catch (const std::out_of_range&) {
    throw Error(MSG("Cursor does not have a sequence '" + cursor + "'"));
  }
}

const std::map<std::string, CursorSequence>& CursorProvider::cursors() const {
  return cursors_;
}

}