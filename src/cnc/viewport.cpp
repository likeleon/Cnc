#include "cnc/stdafx.h"
#include "cnc/stdafx.h"
#include "cnc/viewport.h"

namespace cnc {

Point Viewport::last_mouse_pos_ = Point::Zero;

void Viewport::set_last_mouse_pos(const Point& pos) {
  last_mouse_pos_ = pos;
}

const Point& Viewport::last_mouse_pos() {
  return last_mouse_pos_;
}

}
