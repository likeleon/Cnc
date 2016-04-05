#pragma once

#include "cnc/blend_mode.h"
#include "cnc/sheet_ptr.h"

namespace cnc {

class World;
class WorldRenderer;
class PaletteReference;
class Map;
class Viewport;

class TerrainSpriteLayer {
public:
  TerrainSpriteLayer(World& world, WorldRenderer& wr, SheetPtr sheet, BlendMode blend_mode, PaletteReference& palette, bool restrict_to_bounds);

  void Draw(const Viewport& viewport);

private:
  SheetPtr sheet_;
  BlendMode blend_mode_;
  bool restrict_to_bounds_;
  WorldRenderer& world_renderer_;
  Map& map_;
  PaletteReference& palette_;
};

}
