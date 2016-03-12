#include "cnc/stdafx.h"
#include "cnc/map.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/field_loader.h"
#include <climits>

namespace cnc {

const std::string MapVisibilityTraits::pretty_name = "MapVisibility";

const EnumNamesType<MapVisibilityTraits> MapVisibilityTraits::names = {
  { MapVisibility::Lobby, "Lobby" },
  { MapVisibility::Shellmap, "Shellmap" },
  { MapVisibility::MissionSelector, "MissionSelector" }
};

const std::vector<FieldLoadInfo> Map::LoadInfo = {
  { "Uid", TypeFieldInfo(&Map::uid_) },
  { "Title", TypeFieldInfo(&Map::title_) },
  { "Type", TypeFieldInfo(&Map::type_) },
  { "Description", TypeFieldInfo(&Map::description_) },
  { "Author", TypeFieldInfo(&Map::author_) },
  { "Tileset", TypeFieldInfo(&Map::tileset_) },
};

Map::Map(const std::string& path)
  : path_(path) {
  container_ = Game::mod_data()->mod_files().OpenPackage(path, "", std::numeric_limits<int32_t>::max());
  
  AssertExists("map.yaml");
  AssertExists("map.bin");

  MiniYaml yaml("", MiniYaml::FromStream(container_->GetContent("map.yaml"), path));
  FieldLoader::Load(*this, yaml);
}

void Map::AssertExists(const std::string& filename) {
  auto s = container_->GetContent(filename);
  if (s == nullptr) {
    throw Error(MSG("Required file " + filename + " not present in this map"));
  }
}

}