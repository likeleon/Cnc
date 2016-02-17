#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/install_logic.h"
#include "cnc/widget.h"
#include "cnc/widget_args.h"

namespace cnc {
namespace mods {
namespace common {

const std::vector<std::string> InstallLogic::CtorArgNames = { "widget", "mirror_list_url", "mod_id" };

InstallLogic::InstallLogic(const WidgetPtr& widget, const std::string& mirror_list_url, const std::string& mod_id) {
  //auto panel = widget->Get("INSTALL_PANEL");
  (widget);
  (mirror_list_url);
  (mod_id);
}

}
}
}