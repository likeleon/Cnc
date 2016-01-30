#pragma once

#include "cnc/iinput_handler.h"

namespace cnc {

class NullInputHandler : public IInputHandler {
public:
  void ModifierKeys(Modifiers /*mods*/) override {}
  void OnMouseInput(const MouseInput& /*input*/) override {}
};

class DefaultInputHandler : public IInputHandler {
public:
  void ModifierKeys(Modifiers /*mods*/) override;
  void OnMouseInput(const MouseInput& input) override;
};

}