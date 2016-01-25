#pragma once

#include "cnc/point.h"

namespace cnc {

class Viewport {
public:
  static void set_last_mouse_pos(const Point& pos);
  static const Point& last_mouse_pos();

private:
  static Point last_mouse_pos_;
};

}
