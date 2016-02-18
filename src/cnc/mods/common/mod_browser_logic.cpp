#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/mod_browser_logic.h"
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
#include "cnc/bitmap.h"
#include "cnc/mods/common/rgba_sprite_widget.h"
#include "cnc/mods/common/label_widget.h"

namespace cnc {
namespace mods {
namespace common {

const std::vector<std::string> ModBrowserLogic::CtorArgNames = { "widget" };

static bool IsModInstalled(const ModMetadata* mod) {
  auto file_exists = [](const std::string& file) {
    return File::Exists(Platform::ResolvePath(file));
  };
  return std::all_of(mod->content.test_files.begin(), mod->content.test_files.end(), file_exists);
}

ModBrowserLogic::ModBrowserLogic(const WidgetPtr& widget)
  : mod_install_status_(IsModInstalled),
  mod_prerequisites_fullfilled_(static_cast<bool(*)(const std::string&)>(Game::IsModInstalled)) {

  mod_chooser_panel_ = widget;
  load_button_ = mod_chooser_panel_->Get<ButtonWidget>("LOAD_BUTTON");
  load_button_->on_click_ = [this]() { LoadMod(*selected_mod_); };
  load_button_->is_disabled_ = [this]() { return selected_mod_->id == Game::mod_data()->manifest().mod().id; };

  mod_chooser_panel_->Get<ButtonWidget>("QUIT_BUTTON")->on_click_ = Game::Exit;

  mod_list_ = mod_chooser_panel_->Get("MOD_LIST");
  mod_template_ = mod_list_->Get<ButtonWidget>("MOD_TEMPLATE");

  mod_chooser_panel_->Get<LabelWidget>("MOD_DESC")->get_text_ = [this]() { return selected_description_; };
  mod_chooser_panel_->Get<LabelWidget>("MOD_TITLE")->get_text_ = [this]() { return selected_mod_->title; };
  mod_chooser_panel_->Get<LabelWidget>("MOD_AUTHOR")->get_text_ = [this]() { return selected_author_; };
  mod_chooser_panel_->Get<LabelWidget>("MOD_VERSION")->get_text_ = [this]() { return selected_mod_->version; };

  auto prev_mod = mod_chooser_panel_->Get<ButtonWidget>("PREV_MOD");
  prev_mod->on_click_ = [this]() { mod_offset_ -= 1; RebuildModList(); };
  prev_mod->is_visible_ = [this]() { return mod_offset_ > 0; };

  auto next_mod = mod_chooser_panel_->Get<ButtonWidget>("NEXT_MOD");
  next_mod->on_click_ = [this]() { mod_offset_ += 1; RebuildModList(); };
  next_mod->is_visible_ = [this]() { return mod_offset_ + 5 < static_cast<int32_t>(all_mods_.size()); };

  mod_chooser_panel_->Get<RGBASpriteWidget>("MOD_PREVIEW")->get_sprite_ = [this]() {
    auto iter = previews_.find(selected_mod_->id);
    return iter != previews_.end() ? &iter->second : nullptr;
  };

  sheet_builder_ = std::make_unique<SheetBuilder>(SheetType::BGRA);
  for (const auto& kvp : ModMetadata::AllMods()) {
    if (!kvp.second.hidden) {
      all_mods_.emplace_back(&kvp.second);
    }
  }
  std::sort(all_mods_.begin(), all_mods_.end(),
            [](const auto& a, const auto& b) { return a->title < b->title; });

  for (const auto& mod : all_mods_) {
    try {
      Bitmap preview(Platform::ResolvePaths({ ModMetadata::CandidateModPaths().at(mod->id), "preview.png" }));
      if (preview.Size().width == 296 && preview.Size().height == 196) {
        previews_.emplace(mod->id, sheet_builder_->Add(preview));
      }
    } catch (...) {
    }

    try {
      Bitmap logo(Platform::ResolvePaths({ ModMetadata::CandidateModPaths().at(mod->id), "logo.png" }));
      if (logo.Size().width == 96 && logo.Size().height == 96) {
        logos_.emplace(mod->id, sheet_builder_->Add(logo));
      }
    } catch (...) {
    }
  }

  const ModMetadata* initial_mod = nullptr;
  auto iter = ModMetadata::AllMods().find(Game::settings().game().previous_mod);
  if (iter != ModMetadata::AllMods().end()) {
    initial_mod = &iter->second;
  }
  SelectMod(initial_mod != nullptr && initial_mod->id != "modchooser" ? *initial_mod : ModMetadata::AllMods().at("cnc"));

  RebuildModList();
}

ModBrowserLogic::~ModBrowserLogic() = default;

void ModBrowserLogic::LoadMod(const ModMetadata& mod) {
  if (!mod_prerequisites_fullfilled_[mod.id]) {
    WidgetArgs widget_args({ { "modId", mod.id } });
    Ui::OpenWindow("INSTALL_MOD_PANEL", widget_args);
    return;
  }

  if (!mod_install_status_[&mod]) {
    WidgetArgs widget_args({
      { "mirror_list_url", mod.content.package_mirror_list },
      { "mod_id", mod.id }
    });
    
    Ui::OpenWindow("INSTALL_PANEL", widget_args);
    return;
  }

  Game::RunAfterTick([this, &mod]() {
    Ui::CloseWindow();
    sheet_builder_ = nullptr;
    Game::InitializeMod(mod.id, Arguments::Empty);
  });
}

void ModBrowserLogic::SelectMod(const ModMetadata& mod) {
  selected_mod_ = &mod;
  selected_author_ = "By " + !mod.author.empty() ? mod.author : "unknown author";
  selected_description_ = StringUtils::Replace(mod.description, "\\n", "\n");
  auto selected_index = std::distance(all_mods_.begin(), std::find(all_mods_.begin(), all_mods_.end(), &mod));
  if (selected_index - mod_offset_ > 4) {
    mod_offset_ = selected_index - 4;
  }

  load_button_->text_ = !mod_prerequisites_fullfilled_[mod.id] ? "Install mod" :
    mod_install_status_[&mod] ? "Load Mod" : "Install Assets";
}

void ModBrowserLogic::RebuildModList() {
  mod_list_->RemoveChildren();

  int32_t width = mod_template_->bounds().width;
  int32_t height = mod_template_->bounds().height;
  int32_t inner_margin = mod_template_->bounds().Left();
  int32_t outer_margin = (mod_list_->bounds().width - std::min<int32_t>(5, all_mods_.size()) * width - 4 * inner_margin) / 2;
  int32_t stride = width + inner_margin;

  for (int32_t i = 0; i < 5; ++i) {
    auto j = i + mod_offset_;
    if (j >= static_cast<int32_t>(all_mods_.size())) {
      break;
    }

    const auto* mod = all_mods_[j];

    auto item = std::static_pointer_cast<ButtonWidget>(Widget::Clone(mod_template_));
    item->set_bounds({ outer_margin + i * stride, 0, width, height });
    item->is_highlighted_ = [this, mod]() { return selected_mod_ == mod; };
    item->on_click_ = [this, mod]() { SelectMod(*mod); };
    item->on_double_click_ = [this, mod]() { LoadMod(*mod); };

    auto iter = logos_.find(mod->id);
    const Sprite* logo = (iter != logos_.end()) ? &iter->second : nullptr;
    item->Get<RGBASpriteWidget>("MOD_LOGO")->get_sprite_ = [logo]() { return logo; };
    item->Get("MOD_NO_LOGO")->is_visible_ = [logo]() { return logo == nullptr; };

    mod_list_->AddChild(item);
  }
}

}
}
}