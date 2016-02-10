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
    auto unique_obj = object_creator.CreateObject<T>(loader_name, {});
    loaders.emplace_back(unique_obj.release());
  }
  return loaders;
}

ModData::ModData(const std::string& mod, bool use_load_screen)
  : manifest_(mod) {
  PrepareObjectCreator();

  if (use_load_screen) {
    load_screen_ = object_creator_.CreateObject<ILoadScreen>(manifest_.load_screen().value(), {});
    auto element_selector = [](const auto& y) -> std::string { return y.value(); };
    auto init_info = manifest_.load_screen().ToMap<std::string>(element_selector);
    load_screen_->Init(manifest_, init_info);
    load_screen_->Display();
  }

  widget_loader_ = std::make_unique<WidgetLoader>(*this);
  sprite_loaders_ = GetLoaders<ISpriteLoader>(object_creator_, manifest_.sprite_formats(), "sprite");
}

ModData::~ModData() = default;

void ModData::PrepareObjectCreator() {
  object_creator_.Register<ContainerWidget>("ContainerWidget");
 
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

  cursor_provider_ = std::make_unique<CursorProvider>(*this);
}

const Manifest& ModData::manifest() const {
  return manifest_;
}

ObjectCreator& ModData::object_creator() {
  return object_creator_;
}

const std::vector<SpriteLoaderPtr>& ModData::sprite_loaders() {
  return sprite_loaders_;
}

ILoadScreen* ModData::load_screen() {
  return load_screen_.get();
}

WidgetLoader& ModData::widget_loader() {
  return *widget_loader_;
}

CursorProvider* ModData::cursor_provider() {
  return cursor_provider_.get();
}

FileSystem& ModData::mod_files() {
  return mod_files_;
}

}