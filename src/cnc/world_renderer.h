#pragma once

#include "cnc/viewport.h"
#include "cnc/hardware_palette.h"
#include "cnc/palette_reference.h"
#include "cnc/action.h"

namespace cnc {

class World;
class TerrainRenderer;

class CNC_API WorldRenderer {
public:
  explicit WorldRenderer(World& world);
  ~WorldRenderer();

  void AddPalette(const std::string& name, ImmutablePalettePtr pal, bool allow_modifiers = false, bool allow_overwrite = false);
  void ReplacePalette(const std::string& name, IPalettePtr pal);

  const Viewport& viewport() const { return viewport_; }
  PaletteReference& Palette(const std::string& name);

  void Draw();
  
  Action<> palette_invalidated_;

  WorldRenderer(const WorldRenderer&) = delete;
  WorldRenderer& operator=(const WorldRenderer&) = delete;
  WorldRenderer& operator=(WorldRenderer&&) = delete;

private:
  PaletteReference CreatePaletteReference(const std::string& name);

  World& world_;
  Viewport viewport_;
  HardwarePalette palette_;
  std::map<std::string, PaletteReference> palettes_;
  std::unique_ptr<TerrainRenderer> terrain_renderer_;
};

}
