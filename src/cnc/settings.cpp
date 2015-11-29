#include "cnc/stdafx.h"
#include "cnc/settings.h"
#include "cnc/mini_yaml.h"
#include "cnc/platform.h"
#include "cnc/field_loader.h"

namespace cnc {

const std::vector<FieldLoadInfo> PlayerSettings::load_info_ = {
  { "Name", StringFieldInfo<PlayerSettings>(&PlayerSettings::name) }
};

Settings::Settings(const std::string& path, const Arguments& /*args*/)
  : settings_file_(path) {
  sections_ = {
    { "Player", &player_ }
  };

  if (Platform::Exists(settings_file_)) {
    auto yaml = MiniYaml::MapFromFile(settings_file_);

    for (const auto& kv : sections_) {
      auto iter = yaml.find("Player");
      if (iter != yaml.end()) {
        FieldLoader::Load(*kv.second, iter->second, kv.second->load_info());
      }
    }
  }
}

}
