#include "cnc/stdafx.h"
#include "cnc/sdl2_input.h"
#include "cnc/game.h"
#include "cnc/error_handler.h"

namespace cnc {

static Modifiers MakeModifiers(int32_t raw) {
  auto ret = ((int)((raw & (int)KMOD_ALT) != 0 ? (int)Modifiers::Alt : 0)
            | (int)((raw & (int)KMOD_CTRL) != 0 ? (int)Modifiers::Ctrl : 0)
            | (int)((raw & (int)KMOD_LGUI) != 0 ? (int)Modifiers::Meta : 0)
            | (int)((raw & (int)KMOD_RGUI) != 0 ? (int)Modifiers::Meta : 0)
            | (int)((raw & (int)KMOD_SHIFT) != 0 ? (int)Modifiers::Shift : 0));
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
    case SDL_EventType::SDL_QUIT:
      Game::Exit();
      break;
    case SDL_EventType::SDL_MOUSEMOTION:
      pending_motion = MouseInput(MouseInputEvent::Move, last_button_bits_, scroll_delta, { e.motion.x, e.motion.y }, mods, 0);
      break;
    }
  }

  if (pending_motion) {
    input_handler.OnMouseInput(pending_motion.value());
    pending_motion = {};
  }

  ErrorHandler::CheckGlError();
}

}