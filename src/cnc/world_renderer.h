#pragma once

#include "cnc/viewport.h"

namespace cnc {

class World;

class WorldRenderer {
public:
  explicit WorldRenderer(World& world);

  const Viewport& viewport() const { return viewport_; }

  void Draw();

private:
  World& world_;
  Viewport viewport_;
};

}
