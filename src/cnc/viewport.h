#pragma once

#include "cnc/point.h"

namespace cnc {

class Viewport {
 public:
  static int32_t ticks_since_last_move_;
  static Point last_mouse_pos_;
};

}
