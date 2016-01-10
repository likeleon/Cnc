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
#include "cnc/mods/common/mod_browser_logic.h"

extern "C"
void __declspec(dllexport) RegisterTypes(cnc::ObjectCreator& object_creator) {
  using namespace cnc::mods::common;
  object_creator.Register("ModChooserLoadScreen", &cnc::NewWithoutArgs<ModChooserLoadScreen>);
  object_creator.Register("BackgroundWidget", &cnc::NewWithoutArgs<BackgroundWidget>);
  object_creator.Register("LabelWidget", &cnc::NewWithoutArgs<LabelWidget>);
  object_creator.Register("ButtonWidget", &cnc::NewWithoutArgs<ButtonWidget>);
  object_creator.Register("RGBASpriteWidget", &cnc::NewWithoutArgs<RGBASpriteWidget>);
  object_creator.Register("ImageWidget", &cnc::NewWithoutArgs<ImageWidget>);
  object_creator.Register("TooltipContainerWidget", &cnc::NewWithoutArgs<TooltipContainerWidget>);
  object_creator.Register("ModBrowserLogic", &cnc::New<ModBrowserLogic>);
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

