#include "cnc/mods/common/stdafx.h"
#include "cnc/widget_args.h"
#include "cnc/any.h"
#include "cnc/mods/common/mod_browser_logic.h"

namespace cnc {
namespace mods {
namespace common {

ModBrowserLogic::ModBrowserLogic(const std::map<std::string, Any>& args) {
  mod_chooser_panel_ = AnyCast<WidgetPtr>(args.at("widget"));
}

}
}
}