#pragma once

#include "cnc/field_load_info.h"

namespace cnc {

struct ContentInstaller {
  std::vector<std::string> test_files;
  std::string package_mirror_list;
  std::string music_package_mirror_list;
  int32_t shipped_soundtracks = 0;
  
  static const std::vector<FieldLoadInfo> load_info;
};

}