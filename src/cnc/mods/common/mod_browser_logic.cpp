#include "cnc/mods/common/stdafx.h"
#include "cnc/widget_args.h"
#include "cnc/any.h"
#include "cnc/game.h"
#include "cnc/mods/common/mod_browser_logic.h"

namespace cnc {
namespace mods {
namespace common {

const std::vector<std::string> ModBrowserLogic::CtorArgNames = {
  "widget"
};

ModBrowserLogic::ModBrowserLogic(const WidgetPtr& widget) {
  mod_chooser_panel_ = widget;
  
  load_button_ = mod_chooser_panel_->Get<ButtonWidget>("LOAD_BUTTON");

  mod_chooser_panel_->Get<ButtonWidget>("QUIT_BUTTON")->on_click_ = Game::Exit;
}

}
}
}