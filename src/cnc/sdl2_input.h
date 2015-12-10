#pragma once

namespace cnc {

class IInputHandler;

class Sdl2Input {
public:
  void PumpInput(IInputHandler& input_handler);
};

}
