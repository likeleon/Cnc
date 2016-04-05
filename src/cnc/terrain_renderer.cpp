#include "cnc/stdafx.h"
#include "cnc/terrain_renderer.h"
#include "cnc/world.h"
#include "cnc/world_renderer.h"
#include "cnc/blend_mode.h"
#include "cnc/terrain_sprite_layer.h"

namespace cnc {

TerrainRenderer::TerrainRenderer(World &world, WorldRenderer& wr) {
  terrain_ = std::make_unique<TerrainSpriteLayer>(world, wr, nullptr, BlendMode::Alpha, wr.Palette("terrain"), true);
}

TerrainRenderer::~TerrainRenderer() {
}

void TerrainRenderer::Draw(WorldRenderer& /*wr*/, const Viewport& viewport) {
  terrain_->Draw(viewport);
}

}