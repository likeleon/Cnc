#pragma once

#include "cnc/point.h"

namespace cnc {

enum class MouseInputEvent {
  Down,
  Move,
  Up,
  Scroll
};

enum class MouseButton {
  None = 0,
  Left = 1,
  Right = 2,
  Middle = 4
};

enum class Modifiers {
  None = 0,
  Shift = 1,
  Alt = 2,
  Ctrl = 4,
  Meta = 8
};


struct MouseInput {
  MouseInputEvent event;
  MouseButton button;
  int scroll_delta;
  Point location;
  Modifiers modifiers;
  int multi_tap_count;

  MouseInput(MouseInputEvent event,
             MouseButton button,
             int scroll_delta,
             Point&& location,
             Modifiers modifiers,
             int multi_tap_count)
    : event(event),
    button(button),
    scroll_delta(scroll_delta),
    location(std::move(location)),
    modifiers(modifiers),
    multi_tap_count(multi_tap_count) {
  }
};

class CNC_API IInputHandler {
public:
  virtual ~IInputHandler() {}

  virtual void ModifierKeys(Modifiers mods) = 0;
  virtual void OnMouseInput(const MouseInput& input) = 0;
};

}
