#include "cnc/stdafx.h"
#include "cnc/map_cache.h"
#include "cnc/map_preview.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"
#include "cnc/string_utils.h"
#include "cnc/platform.h"
#include "cnc/directory.h"
#include "cnc/container_utils.h"
#include "cnc/map_preview.h"
#include "cnc/perf_timer.h"
#include "cnc/log.h"
#include "cnc/map.h"

namespace cnc {

MapCache::MapCache(const ModData& mod_data)
  : mod_data_(mod_data), previews_([this](const auto& uid) { return LoadMapPreview(uid); }) {
}

MapCache::~MapCache() {
}

std::unique_ptr<MapPreview> MapCache::LoadMapPreview(const std::string& uid) {
  return std::make_unique<MapPreview>(uid, *this);
}

static std::vector<std::string> FindMapsIn(std::string dir) {
  if (StringUtils::StartsWith(dir, "~")) {
    dir = dir.substr(1);
  }

  dir = Platform::ResolvePath(dir);

  if (!Directory::Exists(dir)) {
    return{};
  }

  std::vector<std::string> dirs_with_maps;
  for (const auto& d : Directory::GetDirectories(dir)) {
    auto files = Directory::GetFiles(d);
    if (std::find(files.begin(), files.end(), "map.yaml") != files.end() &&
        std::find(files.begin(), files.end(), "map.bin") != files.end()) {
      dirs_with_maps.emplace_back(d);
    }
  }
  for (const auto& f : Directory::GetFiles(dir)) {
    if (StringUtils::EndsWith(f, ".oramap")) {
      dirs_with_maps.emplace_back(f);
    }
  }
  return dirs_with_maps;
}

void MapCache::LoadMaps() {
  std::vector<std::pair<std::string, MapClassification>> map_paths;
  for (const auto& kvp : mod_data_.manifest().map_folders()) {
    for (const auto& m : FindMapsIn(kvp.first)) {
      auto classification = kvp.second.empty() ? MapClassification::Unknown : NameToEnum<MapClassificationTraits>(kvp.second);
      map_paths.emplace_back(std::make_pair(m, classification));
    }
  }

  for (const auto& path : map_paths) {
    try {
      PerfTimer perf_timer(path.first);
      auto map = std::make_shared<Map>(path.first);
      previews_[map->uid()]->UpdateFromMap(map, path.second);
    } catch (const std::exception& e) {
      std::cout << "Failed to load map: " << path.first << std::endl;
      std::cout << "Details: " << e.what() << std::endl;
      Log::Write("debug", "Failed to load map: " + path.first);
      Log::Write("debug", "Details: " + std::string(e.what()));
    }
  }
}

std::vector<const MapPreview*> MapCache::Previews() const {
  std::vector<const MapPreview*> ret;
  for (const auto& p : previews_) {
    ret.emplace_back(p.second.get());
  }
  return ret;
}

}