#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/mod_chooser_load_screen.h"
#include "cnc/file.h"
#include "cnc/sprite.h"
#include "cnc/sheet.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/input_handler.h"
#include "cnc/widget.h"
#include "cnc/widget_args.h"
#include "cnc/widget_utils.h"
#include "cnc/arguments.h"

namespace cnc {
namespace mods {
namespace common {

static NullInputHandler null_input_handler;

void ModChooserLoadScreen::Init(const Manifest& /*m*/, const std::map<std::string, std::string>& info) {
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

  r->BeginFrame(Point::Zero, 1.0f);
  WidgetUtils::FillRectWithSprite(bounds_, *sprite_);
  r->EndFrame(null_input_handler);
}

void ModChooserLoadScreen::StartGame(const Arguments& args) {
  WidgetArgs widget_args;

  Ui::LoadWidget("MODCHOOSER_BACKGROUND", Ui::root(), widget_args);

  if (args.Contains("installMusic")) {
    widget_args.Add("modId", args.GetValue("installMusic", ""));
    Ui::OpenWindow("INSTALL_MUSIC_PANEL", widget_args);
  } else {
    Ui::OpenWindow("MODCHOOSER_DIALOG", widget_args);
  }
}

}
}
}