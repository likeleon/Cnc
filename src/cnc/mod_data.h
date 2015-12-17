#pragma once

#include "cnc/manifest.h"
#include "cnc/object_creator.h"

namespace cnc {

class Arguments;

class ILoadScreen {
public:
  virtual ~ILoadScreen() {}

  virtual void Init(const Manifest& m, const std::unordered_map<std::string, std::string>& info) = 0;
  virtual void Display() = 0;
  virtual void StartGame(const Arguments& args) = 0;
};

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

  void PrepareObjectCreator();

  Manifest manifest_;
  ObjectCreator object_creator_;
  std::vector<LibraryPtr> loaded_libraries_;
  std::unique_ptr<ILoadScreen> load_screen_;
};

}