#include "cnc/stdafx.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"

namespace cnc {

using RegisterTypeFunc = void(*)(ObjectCreator& object_creator);

ModData::ModData(const std::string& mod, bool /*use_load_screen*/)
  : manifest_(mod) {

  for (const auto& assembly : manifest_.assemblies()) {
    LibraryPtr library(LoadLibraryA(assembly.c_str()));
    if (!library) {
      Debug::Error("Failed to load library: " + assembly);
      continue;
    }

    auto register_type_func = reinterpret_cast<RegisterTypeFunc>(GetProcAddress(library.get(), "RegisterTypes"));
    if (register_type_func != nullptr) {
      register_type_func(object_creator_);
    }

    loaded_libraries_.emplace_back(std::move(library));
  }
}

}