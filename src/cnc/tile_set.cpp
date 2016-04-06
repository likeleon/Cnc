#include "cnc/stdafx.h"
#include "cnc/tile_set.h"
#include "cnc/field_loader.h"
#include "cnc/field_load_info.h"
#include "cnc/mini_yaml.h"

namespace cnc {

TileSet::TileSet(const std::string& filepath) {
  auto yaml = MiniYaml::MapFromFile(filepath);
  
  FieldLoader::Load(*this, yaml.at("General"));
}

std::vector<FieldLoadInfo> TileSet::GetLoadInfo() const {
  return{
    { "Name", TypeFieldInfo(&TileSet::name_) },
    { "Id", TypeFieldInfo(&TileSet::id_) },
    { "SheetSize", TypeFieldInfo(&TileSet::sheet_size_) },
    { "Palette", TypeFieldInfo(&TileSet::palette_) },
    { "PlayerPalette", TypeFieldInfo(&TileSet::player_palette_) },
    { "IgnoreTileSpriteOffsets", TypeFieldInfo(&TileSet::ignore_tile_sprite_offsets_) },
    { "EnableDepth", TypeFieldInfo(&TileSet::enable_depth_) },
  };
};

}