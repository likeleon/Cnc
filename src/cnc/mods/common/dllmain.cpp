// dllmain.cpp : Defines the entry point for the DLL application.
#include "cnc/mods/common/stdafx.h"
#include "cnc/object_creator.h"
#include "cnc/mod_data.h"
#include "cnc/sheet.h"

namespace cnc {
namespace mods {
namespace common {

class ModChooserLoadScreen : public ILoadScreen {
public:
  ModChooserLoadScreen() {
  }

  void Init(const Manifest& /*m*/, const std::unordered_map<std::string, std::string>& info) override {
    auto sheet = std::make_unique<Sheet>(SheetType::BGRA, info.at("Image"));
  }
  
  void Display() override {
  }
  
  void StartGame(const Arguments & /*args*/) override {
  }
};

}
}
}

extern "C"
void __declspec(dllexport) RegisterTypes(cnc::ObjectCreator& object_creator) {
  object_creator.Register("ModChooserLoadScreen", &cnc::New<cnc::mods::common::ModChooserLoadScreen>);
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

