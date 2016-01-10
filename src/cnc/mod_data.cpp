#include "cnc/stdafx.h"
#include "cnc/mod_data.h"
#include "cnc/manifest.h"
#include "cnc/error.h"
#include "cnc/widget.h"
#include "cnc/widget_loader.h"
#include "cnc/file_system.h"
#include "cnc/chrome_metrics.h"
#include "cnc/chrome_provider.h"

namespace cnc {

using RegisterTypeFunc = void(*)(ObjectCreator& object_creator);

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
}

void ModData::PrepareObjectCreator() {
  object_creator_.Register<ContainerWidget>("ContainerWidget");
 
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

void ModData::MountFiles() {
  mod_files_.LoadFromManifest(manifest_);
}

void ModData::InitializeLoaders() {
  ChromeMetrics::Initialize(manifest_.chrome_metrics());
  ChromeProvider::Initialize(manifest_.chrome());
}

const Manifest& ModData::manifest() const {
  return manifest_;
}

ObjectCreator& ModData::object_creator() {
  return object_creator_;
}

ILoadScreen* ModData::load_screen() {
  return load_screen_.get();
}

WidgetLoader& ModData::widget_loader() {
  return *widget_loader_;
}

FileSystem& ModData::mod_files() {
  return mod_files_;
}

}