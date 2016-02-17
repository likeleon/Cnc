#pragma once

#include "cnc/chrome_logic.h"
#include "cnc/widget_ptr.h"

namespace cnc {
namespace mods {
namespace common {

class InstallLogic : public ChromeLogic {
public:
  InstallLogic(const InstallLogic&) = delete;
  InstallLogic& operator=(const InstallLogic&) = delete;

  InstallLogic(const WidgetPtr& widget, const std::string& mirror_list_url, const std::string& mod_id);

  static const std::vector<std::string> CtorArgNames;
};

}
}
}