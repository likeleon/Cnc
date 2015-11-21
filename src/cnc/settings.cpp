#include "cnc/stdafx.h"
#include "cnc/settings.h"
#include "cnc/mini_yaml.h"

namespace cnc {

Settings::Settings(const Path& path, const Arguments& /*args*/)
  : settings_file_(path) {
  if (std::tr2::sys::exists(settings_file_)) {
    auto yaml = MiniYaml::MapFromFile(settings_file_);
  }
}

}
