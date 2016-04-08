#include "cnc/stdafx.h"
#include "cnc/world_renderer.h"
#include "cnc/world.h"
#include "cnc/actor.h"
#include "cnc/palette_reference.h"
#include "cnc/container_utils.h"
#include "cnc/terrain_renderer.h"
#include "cnc/palette.h"
#include "cnc/traits_interfaces.h"

namespace cnc {

WorldRenderer::WorldRenderer(World& world)
  : world_(world), viewport_(*this, world.map()) {

  for (const auto& pal : world.trait_dict().ActorsWithTrait<ILoadsPalettes>()) {
    pal.trait->LoadPalettes(*this);
  }
  palette_.Initialize();

  terrain_renderer_ = std::make_unique<TerrainRenderer>(world, *this);
}

WorldRenderer::~WorldRenderer() {
}

void WorldRenderer::AddPalette(const std::string& name, ImmutablePalettePtr pal, bool allow_modifiers, bool allow_overwrite) {
  if (allow_overwrite && palette_.Contais(name)) {
    ReplacePalette(name, pal);
  } else {
    auto old_height = palette_.height();
    palette_.AddPalette(name, pal, allow_modifiers);

    if (old_height != palette_.height() && palette_invalidated_ != nullptr) {
      palette_invalidated_();
    }
  }
}

void WorldRenderer::ReplacePalette(const std::string& name, IPalettePtr pal) {
  palette_.ReplacePalette(name, pal);

  if (palettes_.find(name) != palettes_.end()) {
    palettes_.at(name).set_palette(pal);
  }
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