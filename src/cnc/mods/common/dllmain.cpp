// dllmain.cpp : Defines the entry point for the DLL application.
#include "cnc/mods/common/stdafx.h"
#include "cnc/object_creator.h"
#include "cnc/mods/common/mod_chooser_load_screen.h"
#include "cnc/mods/common/background_widget.h"

extern "C"
void __declspec(dllexport) RegisterTypes(cnc::ObjectCreator& object_creator) {
  object_creator.Register("ModChooserLoadScreen", &cnc::New<cnc::mods::common::ModChooserLoadScreen>);
  object_creator.Register("BackgroundWidget", &cnc::New<cnc::mods::common::BackgroundWidget>);
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

