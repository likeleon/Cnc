#include "cnc/stdafx.h"
#include "cnc/map.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/field_loader.h"
#include "cnc/stream_ptr.h"
#include "cnc/mini_yaml.h"
#include "cnc/memory_stream.h"
#include "cnc/sha1.h"
#include <climits>

namespace cnc {

const std::string MapVisibilityTraits::pretty_name = "MapVisibility";

const EnumNamesType<MapVisibilityTraits> MapVisibilityTraits::names = {
  { MapVisibility::Lobby, "Lobby" },
  { MapVisibility::Shellmap, "Shellmap" },
  { MapVisibility::MissionSelector, "MissionSelector" }
};

std::vector<FieldLoadInfo> Map::GetLoadInfo() const {
  return{
    { "MapFormat", TypeFieldInfo(&Map::map_format_) },
    { "Visibility", EnumFieldInfo<MapVisibilityTraits>(&Map::visibility_) },
    { "Title", TypeFieldInfo(&Map::title_) },
    { "Type", TypeFieldInfo(&Map::type_) },
    { "Description", TypeFieldInfo(&Map::description_) },
    { "Author", TypeFieldInfo(&Map::author_) },
    { "Tileset", TypeFieldInfo(&Map::tileset_) },
  };
};

Map::Map(const std::string& path)
  : path_(path) {
  container_ = Game::mod_data()->mod_files().OpenPackage(path, "", std::numeric_limits<int32_t>::max());
  
  AssertExists("map.yaml");
  AssertExists("map.bin");

  MiniYaml yaml("", MiniYaml::FromStream(container_->GetContent("map.yaml"), path));
  FieldLoader::Load(*this, yaml);

  if (map_format_ < MinimumSupportedMapFormat) {
    std::ostringstream oss;
    oss << "Map format " << map_format_ << " is not supported.\n File:" << path;
    throw Error(MSG(oss.str()));
  }

  //auto nd = yaml.ToMap();
  rule_definitions_ = MiniYaml::NodesOrEmpty(yaml, "Rules");

  uid_ = ComputeHash();
}

void Map::AssertExists(const std::string& filename) {
  auto s = container_->GetContent(filename);
  if (s == nullptr) {
    throw Error(MSG("Required file " + filename + " not present in this map"));
  }
}

std::string Map::ComputeHash() const {
  auto s1 = container_->GetContent("map.yaml");
  auto buffer = s1->ReadBytes(s1->Length());
  
  auto s2 = container_->GetContent("map.bin");
  buffer.resize(buffer.size() + s2->Length());
  s2->ReadBytes(buffer, s1->Length(), s2->Length());

  SHA1 sha1;
  sha1.Update(std::string(buffer.data(), buffer.size()));
  return sha1.Final();
}

}