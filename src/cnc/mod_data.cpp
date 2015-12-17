#include "cnc/stdafx.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"
#include "cnc/error.h"

namespace cnc {

using RegisterTypeFunc = void(*)(ObjectCreator& object_creator);

ModData::ModData(const std::string& mod, bool use_load_screen)
  : manifest_(mod) {
  PrepareObjectCreator();

  if (use_load_screen) {
    load_screen_ = object_creator_.CreateObject<ILoadScreen>(manifest_.load_screen().value());
    auto element_selector = [](const auto& y) -> std::string { return y.value(); };
    auto init_info = manifest_.load_screen().ToMap<std::string>(element_selector);
    load_screen_->Init(manifest_, init_info);
    load_screen_->Display();
  }
}

void ModData::PrepareObjectCreator() {
  for (const auto& assembly : manifest_.assemblies()) {
    LibraryPtr library(LoadLibraryA(assembly.c_str()));
    if (!library) {
      throw Error(MSG("Failed to load library: " + assembly));
    }

    auto register_type_func = reinterpret_cast<RegisterTypeFunc>(GetProcAddress(library.get(), "RegisterTypes"));
    if (register_type_func != nullptr) {
      register_type_func(object_creator_);
    }

    loaded_libraries_.emplace_back(std::move(library));
  }
}

}