#pragma once

#include "cnc/iinput_handler.h"

namespace cnc {

class Sdl2Input {
public:
  void PumpInput(IInputHandler& input_handler);

private:
  MouseButton last_button_bits_ = static_cast<MouseButton>(0);
};

}
