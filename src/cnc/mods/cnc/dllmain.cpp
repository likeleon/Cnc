// dllmain.cpp : Defines the entry point for the DLL application.
#include "cnc/mods/cnc/stdafx.h"
#include "cnc/object_creator.h"
#include "cnc/mods/cnc/cnc_load_screen.h"

extern "C"
void __declspec(dllexport) RegisterTypes(cnc::ObjectCreator& object_creator) {
  using namespace cnc::mods::cnc;
  object_creator.Register<CncLoadScreen>("CncLoadScreen");
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

