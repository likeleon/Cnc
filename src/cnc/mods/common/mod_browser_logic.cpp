#include "cnc/mods/common/stdafx.h"
#include "cnc/widget_args.h"
#include "cnc/any.h"
#include "cnc/game.h"
#include "cnc/path.h"
#include "cnc/file.h"
#include "cnc/platform.h"
#include "cnc/mod_data.h"
#include "cnc/sheet_builder.h"
#include "cnc/arguments.h"
#include "cnc/settings.h"
#include "cnc/mods/common/mod_browser_logic.h"

namespace cnc {
namespace mods {
namespace common {

const std::vector<std::string> ModBrowserLogic::CtorArgNames = {
  "widget"
};

static bool IsModInstalled(const ModMetadata* /*mod*/) {
  // TODO
  return true;
}

ModBrowserLogic::ModBrowserLogic(const WidgetPtr& widget)
  : mod_install_status_(IsModInstalled),
  mod_prerequisites_fullfilled_(static_cast<bool(*)(const std::string&)>(Game::IsModInstalled)) {

  mod_chooser_panel_ = widget;
  load_button_ = mod_chooser_panel_->Get<ButtonWidget>("LOAD_BUTTON");
  load_button_->on_click_ = [this]() { LoadMod(*selected_mod_); };
  load_button_->is_disabled_ = [this]() { return selected_mod_->id == Game::mod_data()->manifest().mod().id; };

  mod_chooser_panel_->Get<ButtonWidget>("QUIT_BUTTON")->on_click_ = Game::Exit;

  sheet_builder_ = std::make_unique<SheetBuilder>(SheetType::BGRA);
  for (const auto& kvp : ModMetadata::AllMods()) {
    if (!kvp.second.hidden) {
      all_mods_.emplace_back(&kvp.second);
    }
  }
  std::sort(all_mods_.begin(), all_mods_.end(),
            [](const auto& a, const auto& b) { return a->title < b->title; });

  const ModMetadata* initial_mod = nullptr;
  auto iter = ModMetadata::AllMods().find(Game::settings().game().previous_mod);
  if (iter != ModMetadata::AllMods().end()) {
    initial_mod = &iter->second;
  }
  SelectMod(initial_mod != nullptr && initial_mod->id != "modchooser" ? *initial_mod : ModMetadata::AllMods().at("cnc"));
}

void ModBrowserLogic::LoadMod(const ModMetadata& mod) {
  if (!mod_prerequisites_fullfilled_[mod.id]) {
    WidgetArgs widget_args({ { "modId", mod.id } });
    Ui::OpenWindow("INSTALL_MOD_PANEL", widget_args);
    return;
  }

  if (!mod_install_status_[&mod]) {
    throw Error(MSG("TODO: Implement install panel"));
  }

  Game::RunAfterTick([this, &mod]() {
    Ui::CloseWindow();
    sheet_builder_ = nullptr;
    Game::InitializeMod(mod.id, Arguments::Empty);
  });
}

void ModBrowserLogic::SelectMod(const ModMetadata& mod) {
  selected_mod_ = &mod;
}

}
}
}