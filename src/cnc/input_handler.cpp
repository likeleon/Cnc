#include "cnc/stdafx.h"
#include "cnc/input_handler.h"
#include "cnc/widget.h"
#include "cnc/game.h"

namespace cnc {

void DefaultInputHandler::ModifierKeys(Modifiers mods) {
  Game::HandleModifierKeys(mods);
}

void DefaultInputHandler::OnMouseInput(const MouseInput& input) {
  Ui::HandleInput(input);
}

}
