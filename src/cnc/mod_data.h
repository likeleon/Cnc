#pragma once

#include "cnc/manifest.h"
#include "cnc/object_creator.h"

namespace cnc {

class ModData {
public:
  ModData(const std::string& mod, bool use_load_screen = false);

private:
  struct LibraryDeleter {
    typedef HMODULE pointer;
    void operator() (HMODULE h) {
      FreeLibrary(h);
    }
  };

  using LibraryPtr = std::unique_ptr<HMODULE, LibraryDeleter>;

  Manifest manifest_;
  ObjectCreator object_creator_;
  std::vector<LibraryPtr> loaded_libraries_;
};

}