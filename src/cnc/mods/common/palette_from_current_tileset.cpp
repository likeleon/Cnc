#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/palette_from_current_tileset.h"
#include "cnc/actor_initializer.h"
#include "cnc/world.h"
#include "cnc/tile_set.h"
#include "cnc/world_renderer.h"
#include "cnc/palette.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"

namespace cnc {
namespace mods {
namespace common {

std::deque<std::type_index> PaletteFromCurrentTilesetInfo::Types() const {
  auto types = ITraitInfo::Types();
  types.emplace_front(typeid(PaletteFromCurrentTilesetInfo));
  return types;
}

std::vector<FieldLoadInfo> PaletteFromCurrentTilesetInfo::GetLoadInfo() const {
  return{
    { "Name", TypeFieldInfo(&PaletteFromCurrentTilesetInfo::name_) },
    { "ShadowIndex", Int32VectorFieldInfo(&PaletteFromCurrentTilesetInfo::shadow_index_) },
    { "AllowModifiers", TypeFieldInfo(&PaletteFromCurrentTilesetInfo::allow_modifiers_) }
  };
}

ITraitPtr PaletteFromCurrentTilesetInfo::Create(const ActorInitializer& init) {
  return std::make_shared<PaletteFromCurrentTileset>(init.world(), SharedFromBase<PaletteFromCurrentTilesetInfo>());
}

PaletteFromCurrentTileset::PaletteFromCurrentTileset(World& world, std::shared_ptr<PaletteFromCurrentTilesetInfo> info)
  : world_(world), info_(info) {
}

std::deque<std::type_index> PaletteFromCurrentTileset::Types() const {
  return{ typeid(PaletteFromCurrentTileset), typeid(ILoadsPalettes) };
}

void PaletteFromCurrentTileset::LoadPalettes(WorldRenderer& wr) {
  auto filename = world_.tile_set().player_palette();
  if (filename.empty()) {
    filename = world_.tile_set().palette();
  }
  wr.AddPalette(info_->name_, std::make_shared<ImmutablePalette>(Game::mod_data()->mod_files().Open(filename), info_->shadow_index_), info_->allow_modifiers_);
}

}
}
}
