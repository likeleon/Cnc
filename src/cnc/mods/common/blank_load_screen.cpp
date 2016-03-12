#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/blank_load_screen.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/input_handler.h"
#include "cnc/widget.h"
#include "cnc/settings.h"

namespace cnc {
namespace mods {
namespace common {

void BlankLoadScreen::Init(const Manifest& /*m*/, const std::map<std::string, std::string>& /*info*/) {
}

void BlankLoadScreen::Display() {
  if (Game::renderer() == nullptr) {
    return;
  }

  Game::renderer()->BeginFrame(Point::Zero, 1.0f);
  Game::renderer()->EndFrame(nih_);
}

void BlankLoadScreen::StartGame(const Arguments& /*args*/) {
  // TODO: LaunchArguments
  Ui::ResetAll();
  // TODO: Game::settings().Save();

  Game::LoadShellMap();
  // TODO: Game::settings().Save();
}

}
}
}
