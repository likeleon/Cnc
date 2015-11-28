#include "cnc/stdafx.h"
#include "cnc/settings.h"
#include "cnc/mini_yaml.h"
#include "cnc/platform.h"

namespace cnc {

Settings::Settings(const std::string& path, const Arguments& /*args*/)
  : settings_file_(path) {
  if (Platform::Exists(settings_file_)) {
    auto yaml = MiniYaml::MapFromFile(settings_file_);
  }
}

}
