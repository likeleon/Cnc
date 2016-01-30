#include "cnc/stdafx.h"
#include "cnc/stdafx.h"
#include "cnc/viewport.h"

namespace cnc {

int32_t Viewport::ticks_since_last_move_ = 0;
Point Viewport::last_mouse_pos_ = Point::Zero;

}
