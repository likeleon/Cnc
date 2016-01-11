#pragma once

#include "cnc/chrome_logic.h"
#include "cnc/widget_ptr.h"
#include "cnc/mods/common/button_widget.h"

namespace cnc {

class Any;

namespace mods {
namespace common {

class ModBrowserLogic : public ChromeLogic {
public:
  ModBrowserLogic(const WidgetPtr& widget);

  static const std::vector<std::string> CtorArgNames;

private:
  WidgetPtr mod_chooser_panel_;
  ButtonWidgetPtr load_button_;
};

}
}
}