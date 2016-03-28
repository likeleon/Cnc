#include "cnc/stdafx.h"
#include "cnc/viewport.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/float2.h"

namespace cnc {

int32_t Viewport::ticks_since_last_move_ = 0;
Point Viewport::last_mouse_pos_ = Point::Zero;

Viewport::Viewport(WorldRenderer& wr, const Map& /*map*/)
  : world_renderer_(wr) {
}

Point Viewport::TopLeft() const {
  return center_location_ - viewport_size_ / 2;
}

Point Viewport::BottomRight() const {
  return center_location_ + viewport_size_ / 2;
}

void Viewport::SetZoom(float value) {
  zoom_ = value;
  viewport_size_ = (1.0f / zoom_ * Float2(Game::renderer()->Resolution())).ToSize();
}

}
