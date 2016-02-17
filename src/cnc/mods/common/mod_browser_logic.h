#pragma once

#include "cnc/chrome_logic.h"
#include "cnc/widget_ptr.h"
#include "cnc/cache.h"
#include "cnc/mods/common/button_widget.h"

namespace cnc {

class Any;
struct Sprite;
struct ModMetadata;
class SheetBuilder;

namespace mods {
namespace common {

class ModBrowserLogic : public ChromeLogic {
public:
  ModBrowserLogic(const WidgetPtr& widget);
  ~ModBrowserLogic();

  ModBrowserLogic(const ModBrowserLogic&) = delete;
  ModBrowserLogic& operator=(const ModBrowserLogic&) = delete;

  static const std::vector<std::string> CtorArgNames;

private:
  void LoadMod(const ModMetadata& mod);
  void SelectMod(const ModMetadata& mod);
  void RebuildModList();

  WidgetPtr mod_list_;
  ButtonWidgetPtr mod_template_;
  std::vector<const ModMetadata*> all_mods_;
  std::map<std::string, Sprite> previews_;
  std::map<std::string, Sprite> logos_;
  Cache<const ModMetadata*, bool> mod_install_status_;
  Cache<std::string, bool> mod_prerequisites_fullfilled_;
  WidgetPtr mod_chooser_panel_;
  ButtonWidgetPtr load_button_;
  std::unique_ptr<SheetBuilder> sheet_builder_;
  const ModMetadata* selected_mod_ = nullptr;
  std::string selected_author_;
  std::string selected_description_;
  int32_t mod_offset_ = 0;
};

}
}
}