#pragma once

#include "cnc/chrome_logic.h"
#include "cnc/widget_ptr.h"

namespace cnc {

class Any;

namespace mods {
namespace common {

class ModBrowserLogic : public ChromeLogic {
public:
  ModBrowserLogic(const std::map<std::string, Any>& args);

private:
  WidgetPtr mod_chooser_panel_;
};

}
}
}