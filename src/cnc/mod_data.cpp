#include "cnc/stdafx.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"
#include "cnc/error.h"
#include "cnc/widget.h"
#include "cnc/widget_loader.h"
#include "cnc/chrome_metrics.h"
#include "cnc/cursor_provider.h"
#include "cnc/chrome_provider.h"
#include "cnc/sprite_loader.h"
#include "cnc/iload_screen.h"
#include "cnc/content_installer.h"
#include "cnc/map_cache.h"
#include "cnc/map_preview.h"
#include "cnc/map.h"
#include "cnc/ruleset_cache.h"
#include "cnc/ruleset.h"

namespace cnc {

using RegisterTypeFunc = void(*)(ObjectCreator& object_creator);

template <typename T>
std::vector<std::shared_ptr<T>> GetLoaders(ObjectCreator& object_creator,
                                           const std::vector<std::string>& formats, 
                                           const std::string& name) {
  std::vector<std::shared_ptr<T>> loaders;
  for (const auto& format : formats) {
    std::string loader_name = format + "Loader";
    if (!object_creator.TypeRegistered(loader_name)) {
      throw Error(MSG("Unable to find a " + name + " loader for type '" + format + "'."));
    }
    loaders.emplace_back(object_creator.CreateObject<T>(loader_name, {}));
  }
  return loaders;
}

ModData::ModData(const std::string& mod, bool use_load_screen)
  : manifest_(mod), default_rules_([this](){ return ruleset_cache_->Load(); }) {
  PrepareObjectCreator();
  manifest_.LoadCustomData(object_creator_);

  if (use_load_screen) {
    load_screen_ = object_creator_.CreateObject<ILoadScreen>(manifest_.load_screen().value(), {});
    auto element_selector = [](const auto& y) -> std::string { return y.value(); };
    auto init_info = manifest_.load_screen().ToMap<std::string>(element_selector);
    load_screen_->Init(manifest_, init_info);
    load_screen_->Display();
  }

  widget_loader_ = std::make_unique<WidgetLoader>(*this);
  ruleset_cache_ = std::make_unique<RulesetCache>(*this);
  map_cache_ = std::make_unique<MapCache>(*this);
  sprite_loaders_ = GetLoaders<ISpriteLoader>(object_creator_, manifest_.sprite_formats(), "sprite");
}

ModData::~ModData() = default;

void ModData::PrepareObjectCreator() {
  object_creator_.Register<ContainerWidget>("ContainerWidget");
  object_creator_.Register<ContentInstaller, const MiniYaml&>("ContentInstaller");
 
  for (const auto& assembly : manifest_.assemblies()) {
    auto module = FileSystem::ResolveLibrary(assembly);
    auto register_type_func = reinterpret_cast<RegisterTypeFunc>(GetProcAddress(module, "RegisterTypes"));
    if (register_type_func != nullptr) {
      register_type_func(object_creator_);
    }
  }
}

void ModData::MountFiles() {
  mod_files_.LoadFromManifest(manifest_);
}

void ModData::InitializeLoaders() {
  ChromeMetrics::Initialize(manifest_.chrome_metrics());
  ChromeProvider::Initialize(manifest_.chrome());

  cursor_provider_ = std::make_shared<CursorProvider>(*this);
}

MapUniquePtr ModData::PrepareMap(const std::string& uid) {
  if (load_screen_ != nullptr) {
    load_screen_->Display();
  }

  if (map_cache_->GetPreview(uid).status() != MapStatus::Available) {
    throw Error(MSG("Invalid map uid: " + uid));
  }

  auto map = std::make_unique<Map>(map_cache_->GetPreview(uid).map().path());

  InitializeLoaders();
  mod_files_.LoadFromManifest(manifest_);

  mod_files_.Mount(mod_files_.OpenPackage(map->path(), "", std::numeric_limits<int32_t>::max()));

  return map;
}

}