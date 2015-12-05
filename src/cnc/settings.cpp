#include "cnc/stdafx.h"
#include "cnc/settings.h"
#include "cnc/mini_yaml.h"
#include "cnc/platform.h"
#include "cnc/field_loader.h"

namespace cnc {

const std::vector<FieldLoadInfo> PlayerSettings::load_info_ = {
  { "Name", StringFieldInfo<PlayerSettings>(&PlayerSettings::name), true }
};

const std::vector<FieldLoadInfo> GraphicSettings::load_info_ = {
  { "Renderer", StringFieldInfo<GraphicSettings>(&GraphicSettings::renderer), true }
};

Settings::Settings(const std::string& path, const Arguments& /*args*/)
  : settings_file_(path) {
  sections_ = {
    { "Player", &player_ },
    { "Graphics", &graphics_ }
  };

  if (Platform::Exists(settings_file_)) {
    auto yaml = MiniYaml::MapFromFile(settings_file_);

    for (const auto& kv : sections_) {
      auto iter = yaml.find(kv.first);
      if (iter != yaml.end()) {
        FieldLoader::Load(*kv.second, iter->second, kv.second->load_info());
      }
    }
  }
}

}
