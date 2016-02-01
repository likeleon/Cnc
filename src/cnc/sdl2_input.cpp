#include "cnc/stdafx.h"
#include "cnc/sdl2_input.h"
#include "cnc/game.h"
#include "cnc/error_handler.h"
#include "cnc/multi_tap_detection.h"

namespace cnc {

static MouseButton MakeButton(char b) {
  return b == SDL_BUTTON_LEFT ? MouseButton::Left
       : b == SDL_BUTTON_RIGHT ? MouseButton::Right
       : b == SDL_BUTTON_MIDDLE ? MouseButton::Middle
       : MouseButton::None;
}

static Modifiers MakeModifiers(int32_t raw) {
  auto ret = (((raw & KMOD_ALT) != 0 ? (int)Modifiers::Alt : 0)
            | ((raw & KMOD_CTRL) != 0 ? (int)Modifiers::Ctrl : 0)
            | ((raw & KMOD_LGUI) != 0 ? (int)Modifiers::Meta : 0)
            | ((raw & KMOD_RGUI) != 0 ? (int)Modifiers::Meta : 0)
            | ((raw & KMOD_SHIFT) != 0 ? (int)Modifiers::Shift : 0));
  return static_cast<Modifiers>(ret);
}

void Sdl2Input::PumpInput(IInputHandler& input_handler) {
  auto mods = MakeModifiers(static_cast<int32_t>(SDL_GetModState()));
  int32_t scroll_delta = 0;
  input_handler.ModifierKeys(mods);
  optional<MouseInput> pending_motion;

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
      case SDL_EventType::SDL_QUIT: {
        Game::Exit();
        break;
      }
      
      case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
        if (pending_motion) {
          input_handler.OnMouseInput(pending_motion.value());
          pending_motion = {};
        }

        auto button = MakeButton(e.button.button);
        last_button_bits_ = (MouseButton)((int)last_button_bits_ | (int)button);

        Point pos(e.button.x, e.button.y);
        input_handler.OnMouseInput(MouseInput(
          MouseInputEvent::Down, button, scroll_delta, pos, mods,
          MultiTapDetection::DetectFromMouse(e.button.button, pos)));
        break;
      }

      case SDL_EventType::SDL_MOUSEBUTTONUP: {
        if (pending_motion) {
          input_handler.OnMouseInput(pending_motion.value());
          pending_motion = {};
        }

        auto button = MakeButton(e.button.button);
        last_button_bits_ = (MouseButton)((int)last_button_bits_ & ~(int)button);

        Point pos(e.button.x, e.button.y);
        input_handler.OnMouseInput(MouseInput(
          MouseInputEvent::Up, button, scroll_delta, pos, mods,
          MultiTapDetection::InfoFromMouse(e.button.button)));
        break;
      }

      case SDL_EventType::SDL_MOUSEMOTION: {
        pending_motion = MouseInput(MouseInputEvent::Move, last_button_bits_, scroll_delta, { e.motion.x, e.motion.y }, mods, 0);
        break;
      }
    }
  }

  if (pending_motion) {
    input_handler.OnMouseInput(pending_motion.value());
    pending_motion = {};
  }

  ErrorHandler::CheckGlError();
}

}