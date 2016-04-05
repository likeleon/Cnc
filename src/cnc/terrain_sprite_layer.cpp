#include "cnc/stdafx.h"
#include "cnc/terrain_sprite_layer.h"
#include "cnc/world.h"
#include "cnc/viewport.h"

namespace cnc {

TerrainSpriteLayer::TerrainSpriteLayer(World& world, WorldRenderer& wr, SheetPtr sheet, BlendMode blend_mode, PaletteReference& palette, bool restrict_to_bounds)
: sheet_(sheet), blend_mode_(blend_mode), restrict_to_bounds_(restrict_to_bounds), world_renderer_(wr), map_(world.map()), palette_(palette) {
}

void TerrainSpriteLayer::Draw(const Viewport& /*viewport*/) {
}

}
