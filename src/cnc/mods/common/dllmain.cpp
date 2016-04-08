// dllmain.cpp : Defines the entry point for the DLL application.
#include "cnc/mods/common/stdafx.h"
#include "cnc/object_creator.h"
#include "cnc/mods/common/mod_chooser_load_screen.h"
#include "cnc/mods/common/background_widget.h"
#include "cnc/mods/common/label_widget.h"
#include "cnc/mods/common/button_widget.h"
#include "cnc/mods/common/rgba_sprite_widget.h"
#include "cnc/mods/common/image_widget.h"
#include "cnc/mods/common/tooltip_container_widget.h"
#include "cnc/mods/common/progress_bar_widget.h"
#include "cnc/mods/common/mod_browser_logic.h"
#include "cnc/mods/common/install_logic.h"
#include "cnc/mods/common/download_packages_logic.h"
#include "cnc/mods/common/shp_td_loader.h"
#include "cnc/mods/common/tmp_td_loader.h"
#include "cnc/mods/common/shp_ts_loader.h"
#include "cnc/mods/common/tmp_ra_loader.h"

#include "cnc/mods/common/actor_lost_notification.h"
#include "cnc/mods/common/always_visible.h"
#include "cnc/mods/common/palette_from_current_tileset.h"

extern "C"
void __declspec(dllexport) RegisterTypes(cnc::ObjectCreator& object_creator) {
  using namespace cnc;
  using namespace cnc::mods::common;

  object_creator.Register<ModChooserLoadScreen>("ModChooserLoadScreen");
  object_creator.Register<BackgroundWidget>("BackgroundWidget");
  object_creator.Register<LabelWidget>("LabelWidget");
  object_creator.Register<ButtonWidget>("ButtonWidget");
  object_creator.Register<RGBASpriteWidget>("RGBASpriteWidget");
  object_creator.Register<ImageWidget>("ImageWidget");
  object_creator.Register<TooltipContainerWidget>("TooltipContainerWidget");
  object_creator.Register<ProgressBarWidget>("ProgressBarWidget");
  object_creator.Register<ModBrowserLogic, const cnc::WidgetPtr&>("ModBrowserLogic");
  object_creator.Register<InstallLogic, const cnc::WidgetPtr&, const std::string&, const std::string&>("InstallLogic");
  object_creator.Register<DownloadPackagesLogic, const cnc::WidgetPtr&, Action<>, const std::string&, const std::string&>("DownloadPackagesLogic");
  object_creator.Register<ShpTDLoader>("ShpTDLoader");
  object_creator.Register<TmpTDLoader>("TmpTDLoader");
  object_creator.Register<ShpTSLoader>("ShpTSLoader");
  object_creator.Register<TmpRALoader>("TmpRALoader");

  object_creator.Register<ActorLostNotificationInfo>("ActorLostNotificationInfo");
  object_creator.Register<AlwaysVisibleInfo>("AlwaysVisibleInfo");
  object_creator.Register<PaletteFromCurrentTilesetInfo>("PaletteFromCurrentTilesetInfo");
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/,
                      DWORD  ul_reason_for_call,
                      LPVOID /*lpReserved*/) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

