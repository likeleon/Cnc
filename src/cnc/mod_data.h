#pragma once

#include "cnc/manifest.h"
#include "cnc/object_creator.h"
#include "cnc/file_system.h"
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

  Manifest& manifest() { return manifest_; }
  const Manifest& manifest() const { return manifest_; }
  ObjectCreator& object_creator() { return object_creator_; }
  const std::vector<SpriteLoaderPtr>& sprite_loaders() { return sprite_loaders_; }
  RulesetCache& ruleset_cache() { return *ruleset_cache_; }
  ILoadScreen* load_screen() { return load_screen_.get(); }
  WidgetLoader& widget_loader() { return *widget_loader_; }
  MapCache& map_cache() { return *map_cache_; }
  std::shared_ptr<CursorProvider> cursor_provider() { return cursor_provider_; }
  FileSystem& mod_files() { return mod_files_; }
  std::shared_ptr<Ruleset> DefaultRules() { return default_rules_(); }

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
  Lazy<Func<std::shared_ptr<Ruleset>>> default_rules_;
};

}