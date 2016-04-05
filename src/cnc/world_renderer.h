#pragma once

#include "cnc/viewport.h"
#include "cnc/hardware_palette.h"
#include "cnc/palette_reference.h"

namespace cnc {

class World;
class TerrainRenderer;

class WorldRenderer {
public:
  explicit WorldRenderer(World& world);
  ~WorldRenderer();

  const Viewport& viewport() const { return viewport_; }

  PaletteReference& Palette(const std::string& name);

  void Draw();

private:
  PaletteReference CreatePaletteReference(const std::string& name);

  World& world_;
  Viewport viewport_;
  HardwarePalette palette_;
  std::map<std::string, PaletteReference> palettes_;
  std::unique_ptr<TerrainRenderer> terrain_renderer_;
};

}
