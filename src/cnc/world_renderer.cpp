#include "cnc/stdafx.h"
#include "cnc/world_renderer.h"
#include "cnc/world.h"
#include "cnc/actor.h"
#include "cnc/palette_reference.h"
#include "cnc/container_utils.h"
#include "cnc/terrain_renderer.h"

namespace cnc {

WorldRenderer::WorldRenderer(World& world)
  : world_(world), viewport_(*this, world.map()) {
  palette_.Initialize();

  terrain_renderer_ = std::make_unique<TerrainRenderer>(world, *this);
}

WorldRenderer::~WorldRenderer() {
}

PaletteReference& WorldRenderer::Palette(const std::string& name) {
  return GetOrAdd(palettes_, name, [this](const auto& name) { return CreatePaletteReference(name); });
}

PaletteReference WorldRenderer::CreatePaletteReference(const std::string& name) {
  auto pal = palette_.GetPalette(name);
  return PaletteReference(name, palette_.GetPaletteIndex(name), pal, palette_);
}

void WorldRenderer::Draw() {
  if (world_.world_actor().disposed()) {
    return;
  }

  terrain_renderer_->Draw(*this, viewport_);
}

}  