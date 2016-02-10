#include "cnc/mods/cnc/stdafx.h"
#include "cnc/mods/cnc/cnc_load_screen.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/file.h"
#include "cnc/platform.h"
#include "cnc/sheet.h"

namespace cnc {
namespace mods {
namespace cnc {

CncLoadScreen::~CncLoadScreen() {
}

void CncLoadScreen::Init(const Manifest& m, const std::map<std::string, std::string>& info) {
  load_info_ = info;

  r_ = Game::renderer();
  if (r_ == nullptr) {
    return;
  }

  auto stream = File::ReadAllBytes(Platform::ResolvePath(load_info_["Image"]));
  sheet_ = std::make_shared<Sheet>(SheetType::BGRA, stream);

  auto res = r_->Resolution();
  border_top_ = Sprite(sheet_, { 161, 128, 62, 33 }, )
}

}
}
}
