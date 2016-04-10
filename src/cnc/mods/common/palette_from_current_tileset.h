#pragma once

#include "cnc/traits_interfaces.h"

namespace cnc {

class World;

namespace mods {
namespace common {

class PaletteFromCurrentTileset;

class PaletteFromCurrentTilesetInfo : public ITraitInfo {
public:
  ITraitPtr Create(const ActorInitializer& init) override;

  std::deque<std::type_index> Types() const override;
  std::vector<FieldLoadInfo> GetLoadInfo() const override;

  std::string name_;
  std::vector<int32_t> shadow_index_;
  bool allow_modifiers_ = true;
};

class PaletteFromCurrentTileset : public ILoadsPalettes {
public:
  PaletteFromCurrentTileset(World& world, std::shared_ptr<PaletteFromCurrentTilesetInfo> info);
  void LoadPalettes(WorldRenderer& wr) override;
  
  std::deque<std::type_index> Types() const override;

  World& world_;
  std::shared_ptr<PaletteFromCurrentTilesetInfo> info_;
};

}
}
}
