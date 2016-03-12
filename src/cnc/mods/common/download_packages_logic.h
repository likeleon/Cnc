#pragma once

#include "cnc/chrome_logic.h"
#include "cnc/widget_ptr.h"
#include "cnc/action.h"
#include "cnc/mods/common/widget_ptrs.h"

namespace cnc {
namespace mods {
namespace common {

class DownloadPackagesLogic : public ChromeLogic {
public:
  DownloadPackagesLogic(const DownloadPackagesLogic&) = delete;
  DownloadPackagesLogic& operator=(const DownloadPackagesLogic&) = delete;

  DownloadPackagesLogic(const WidgetPtr& widget, Action<> after_install, const std::string& mirror_list_url, const std::string& mod_id);

  static const std::vector<std::string> CtorArgNames;

private:
  void ShowDownloadDialog();

  WidgetPtr panel_;
  std::string mod_id_;
  std::string mirror_list_url_;
  ProgressBarWidgetPtr progress_bar_;
  LabelWidgetPtr status_label_;
  Action<> after_install_;
  std::string mirror_;
};

}
}
}