#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/install_logic.h"
#include "cnc/widget.h"
#include "cnc/widget_args.h"
#include "cnc/game.h"
#include "cnc/arguments.h"
#include "cnc/mod_metadata.h"
#include "cnc/widget.h"
#include "cnc/mods/common/label_widget.h"
#include "cnc/mods/common/button_widget.h"

namespace cnc {
namespace mods {
namespace common {

const std::vector<std::string> InstallLogic::CtorArgNames = { "widget", "mirror_list_url", "mod_id" };

InstallLogic::InstallLogic(const WidgetPtr& widget, const std::string& mirror_list_url, const std::string& mod_id) {
  auto panel = widget->Get("INSTALL_PANEL");
  WidgetArgs widget_args({
    { "after_install", [mod_id]() { Game::InitializeMod(mod_id, Arguments::Empty); } },
    { "mirror_list_url", mirror_list_url },
    { "mod_id", mod_id }
  });

  const auto& mod = ModMetadata::AllMods().at(mod_id);
  auto text = "OpenRA requires the original " + mod.title + " game contents.";
  panel->Get<LabelWidget>("DESC1")->text_ = text;

  auto download_button = panel->Get<ButtonWidget>("DOWNLOAD_BUTTON");
  download_button->on_click_ = [widget_args]() { Ui::OpenWindow("INSTALL_DOWNLOAD_PANEL", widget_args); };
  download_button->is_disabled_ = [&mod]() { return mod.content.package_mirror_list.empty(); };

  panel->Get<ButtonWidget>("INSTALL_BUTTON")->disabled_ = true;

  panel->Get<ButtonWidget>("BACK_BUTTON")->on_click_ = Ui::CloseWindow;
}

}
}
}