#include "cnc/stdafx.h"
#include "cnc/world_renderer.h"
#include "cnc/world.h"

namespace cnc {

WorldRenderer::WorldRenderer(World& world)
  : world_(world), viewport_(*this, world.map()) {
}

void WorldRenderer::Draw() {

}

}