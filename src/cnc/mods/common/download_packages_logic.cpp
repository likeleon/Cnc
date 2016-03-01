#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/download_packages_logic.h"
#include "cnc/widget.h"
#include "cnc/mod_metadata.h"
#include "cnc/platform.h"
#include "cnc/path.h"
#include "cnc/download.h"
#include "cnc/game.h"
#include "cnc/string_utils.h"
#include "cnc/mods/common/progress_bar_widget.h"
#include "cnc/mods/common/label_widget.h"
#include "cnc/mods/common/button_widget.h"
#include "cnc/mods/common/install_utils.h"
#include <cmath>
#include <iomanip>
#include <random>

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

  ShowDownloadDialog();
}

static std::vector<std::string> SizeSuffixes = {
  "bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"
};

static double LogWithBase(double x, double base) {
  return std::log(x) / std::log(base);
}

void DownloadPackagesLogic::ShowDownloadDialog() {
  status_label_->get_text_ = [] { return "Fetching list of mirrors..."; };
  progress_bar_->indeterminate_ = true;

  auto retry_button = panel_->Get<ButtonWidget>("RETRY_BUTTON");
  retry_button->is_visible_ = [] { return false; };

  auto cancel_button = panel_->Get<ButtonWidget>("CANCEL_BUTTON");
  (cancel_button);
  
  auto file = Path::Combine({ Path::GetTempPath(), Path::GetRandomFileName() });
  auto dest = Platform::ResolvePaths({ "^", "Content", mod_id_ });

  Action<DownloadProgressChanged> on_download_progress = [=](DownloadProgressChanged i) {
    auto data_received = 0.0f;
    auto data_total = 0.0f;
    size_t mag = 0;

    if (i.total_bytes_to_receive < 0) {
      data_received = static_cast<float>(i.bytes_received);
    } else {
      mag = static_cast<size_t>(LogWithBase(static_cast<double>(i.total_bytes_to_receive), 1024));
      data_total = i.total_bytes_to_receive / static_cast<float>(1L << (mag * 10));
      data_received = i.bytes_received / static_cast<float>(1L << (mag * 10));
    }

    progress_bar_->indeterminate_ = false;
    progress_bar_->percentage_ = i.percentage_;

    status_label_->get_text_ = [=]() {
      auto mirror = !mirror_.empty() ? mirror_ : "unknown host";
      return "Downloading from " + mirror;
      /*std::ostringstream oss;
      auto progress = std::to_string(data_received) + "/" + std::to_string(data_total) + " " + SizeSuffixes[mag];
      oss << "Downloading from " << mirror << " " << progress << " (" << i.percentage_ << "%%)";
      return oss.str();*/
    };
  };

  Action<std::string> on_extract_progress = [=](const std::string& s) {
    Game::RunAfterTick([=] { status_label_->get_text_ = [=] { return s; }; });
  };

  Action<std::string> on_error = [=](const std::string& s) {
    Game::RunAfterTick([=] {
      status_label_->get_text_ = [s] { return "error: " + s; };
      retry_button->is_visible_ = [] { return true; };  
    });
  };

  Action<AsyncCompleted, bool> on_download_complete = [=](AsyncCompleted i, bool cancelled) {
    if (!i.error.empty()) {
      on_error(i.error);
      return;
    }

    if (cancelled) {
      on_error("Download cancelled");
      return;
    }

    status_label_->get_text_ = [] { return "Extracting..."; };
    progress_bar_->indeterminate_ = true;
    if (InstallUtils::ExtractZip(file, dest, on_extract_progress, on_error)) {
      Game::RunAfterTick([=] {
        Ui::CloseWindow();
        after_install_();
      });
    }
  };

  Action<DownloadDataCompleted, bool> on_fetch_mirrors_complete = [=](DownloadDataCompleted i, bool cancelled) {
    progress_bar_->indeterminate_ = true;

    if (!i.error.empty()) {
      on_error(i.error);
      return;
    }

    if (cancelled) {
      on_error("Download cancelled");
      return;
    }

    std::string data(&i.result[0]);
    auto mirror_list = StringUtils::Split(data, '\n', StringSplitOptions::RemoveEmptyEntries);
    std::default_random_engine gen(static_cast<uint32_t>(time(0)));
    std::uniform_int_distribution<> dis(0, mirror_list.size() - 1);
    mirror_ = mirror_list[dis(gen)];

    auto dl = Download::ToFile(mirror_, file, on_download_progress, on_download_complete);
    cancel_button->on_click_ = [=] { dl->Cancel(); Ui::CloseWindow(); };
    retry_button->on_click_ = [=] { dl->Cancel(); ShowDownloadDialog(); };
  };

  auto update_mirros = Download::ToData(mirror_list_url_, on_download_progress, on_fetch_mirrors_complete);
  cancel_button->on_click_ = [=] { update_mirros->Cancel(); Ui::CloseWindow(); };
  retry_button->on_click_ = [=] { update_mirros->Cancel(); ShowDownloadDialog(); };
}

}
}
}