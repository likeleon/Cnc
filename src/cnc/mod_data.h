#pragma once

#include "cnc/manifest.h"
#include "cnc/object_creator.h"
#include "cnc/file_system.h"
#include "cnc/sprite_loader_ptr.h"

namespace cnc {

class WidgetLoader;
class CursorProvider;
class ILoadScreen;

class CNC_API ModData {
public:
  ModData(const std::string& mod, bool use_load_screen = false);
  ~ModData();

  void MountFiles();
  void InitializeLoaders();

  const Manifest& manifest() const;
  ObjectCreator& object_creator();
  const std::vector<SpriteLoaderPtr>& sprite_loaders();
  ILoadScreen* load_screen();
  WidgetLoader& widget_loader();
  CursorProvider* cursor_provider();
  FileSystem& mod_files();

private:
  void PrepareObjectCreator();

  Manifest manifest_;
  ObjectCreator object_creator_;
  std::vector<SpriteLoaderPtr> sprite_loaders_;
  std::unique_ptr<ILoadScreen> load_screen_;
  std::unique_ptr<WidgetLoader> widget_loader_;
  std::unique_ptr<CursorProvider> cursor_provider_;
  FileSystem mod_files_;
};

}