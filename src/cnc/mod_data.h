#pragma once

#include "cnc/manifest.h"
#include "cnc/object_creator.h"
#include "cnc/file_system.h"
#include "cnc/sprite_loader_ptr.h"
#include "cnc/map_ptr.h"
#include "cnc/lazy.h"
#include "cnc/action.h"

namespace cnc {

class WidgetLoader;
class CursorProvider;
class ILoadScreen;
class MapCache;
class Map;
class RulesetCache;
class Ruleset;

class CNC_API ModData {
public:
  ModData(const std::string& mod, bool use_load_screen = false);
  ~ModData();

  void MountFiles();
  void InitializeLoaders();
  MapUniquePtr PrepareMap(const std::string& uid);

  Manifest& manifest();
  const Manifest& manifest() const;
  ObjectCreator& object_creator();
  const std::vector<SpriteLoaderPtr>& sprite_loaders();
  ILoadScreen* load_screen();
  WidgetLoader& widget_loader();
  MapCache& map_cache();
  std::shared_ptr<CursorProvider> cursor_provider();
  FileSystem& mod_files();
  Ruleset& DefaultRules();

private:
  void PrepareObjectCreator();

  Manifest manifest_;
  ObjectCreator object_creator_;
  std::vector<SpriteLoaderPtr> sprite_loaders_;
  std::unique_ptr<RulesetCache> ruleset_cache_;
  std::shared_ptr<ILoadScreen> load_screen_;
  std::unique_ptr<WidgetLoader> widget_loader_;
  std::unique_ptr<MapCache> map_cache_;
  std::shared_ptr<CursorProvider> cursor_provider_;
  FileSystem mod_files_;
  Lazy<Func<std::unique_ptr<Ruleset>>> default_rules_;
};

}