#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/download_packages_logic.h"
#include "cnc/widget.h"
#include "cnc/mod_metadata.h"
#include "cnc/mods/common/progress_bar_widget.h"
#include "cnc/mods/common/label_widget.h"

namespace cnc {
namespace mods {
namespace common {

const std::vector<std::string> DownloadPackagesLogic::CtorArgNames = { "widget", "after_install", "mirror_list_url", "mod_id" };

DownloadPackagesLogic::DownloadPackagesLogic(const WidgetPtr& widget, Action<> after_install, const std::string& mirror_list_url, const std::string& mod_id)
  : mirror_list_url_(mirror_list_url), after_install_(after_install), mod_id_(mod_id) {
  panel_ = widget->Get("INSTALL_DOWNLOAD_PANEL");
  progress_bar_ = panel_->Get<ProgressBarWidget>("PROGRESS_BAR");
  status_label_ = panel_->Get<LabelWidget>("STATUS_LABEL");

  auto text = "Downloading " + ModMetadata::AllMods().at(mod_id).title + " assets...";
  panel_->Get<LabelWidget>("TITLE")->text_ = text;
}

}
}
}