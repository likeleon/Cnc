#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/mod_chooser_load_screen.h"
#include "cnc/file.h"
#include "cnc/sprite.h"
#include "cnc/sheet.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/input_handler.h"
#include "cnc/widget_utils.h"

namespace cnc {
namespace mods {
namespace common {

static NullInputHandler null_input_handler;

void ModChooserLoadScreen::Init(const Manifest& /*m*/, const std::unordered_map<std::string, std::string>& info) {
  auto res = Game::renderer()->Resolution();
  bounds_ = { 0, 0, res.width, res.height };

  auto stream = File::OpenRead(info.at("Image"));
  auto sheet = std::make_shared<Sheet>(SheetType::BGRA, stream);
  sprite_ = std::make_unique<Sprite>(sheet, Rectangle(0, 0, 1024, 480), TextureChannel::Alpha);
}

void ModChooserLoadScreen::Display() {
  auto r = Game::renderer();
  if (r == nullptr) {
    return;
  }

  r->BeginFrame();
  WidgetUtils::FillRectWithSprite(bounds_, *sprite_);
  r->EndFrame(null_input_handler);
}

void ModChooserLoadScreen::StartGame(const Arguments & /*args*/) {
}

}
}
}