#include "cnc/stdafx.h"
#include "cnc/sdl2_input.h"
#include "cnc/game.h"
#include "cnc/error_handler.h"

namespace cnc {

void Sdl2Input::PumpInput(IInputHandler& /*input_handler*/) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_EventType::SDL_QUIT:
      Game::Exit();
      break;
    }
  }

  ErrorHandler::CheckGlError();
}

}