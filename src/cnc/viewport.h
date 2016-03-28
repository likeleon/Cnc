#pragma once

#include "cnc/point.h"
#include "cnc/size.h"

namespace cnc {

class WorldRenderer;
class Map;

class Viewport {
public:
  Viewport(WorldRenderer& wr, const Map& map);

  const Point& center_location() const { return center_location_; }
  Point TopLeft() const;
  Point BottomRight() const;
  
  float zoom() const { return zoom_; }
  void SetZoom(float value);

  static int32_t ticks_since_last_move_;
  static Point last_mouse_pos_;

  Viewport(const Viewport& other) = delete;
  Viewport& operator=(const Viewport&) = delete;
  Viewport& operator=(Viewport&&) = delete;

private:
  WorldRenderer& world_renderer_;
  Point center_location_ = Point::Zero;
  Size viewport_size_;
  float zoom_ = 1.0f;
};

}
