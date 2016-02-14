#pragma once

#include "cnc/iglobal_mod_data.h"
#include "cnc/field_load_info.h"

namespace cnc {

class MiniYaml;

struct ContentInstaller : public IGlobalModData {
  ContentInstaller();
  ContentInstaller(const MiniYaml& yaml);

  static const std::vector<std::string> CtorArgNames;
  static const std::vector<FieldLoadInfo> LoadInfo;

  std::vector<std::string> test_files;
  std::string package_mirror_list;
  std::string music_package_mirror_list;
  int32_t shipped_soundtracks = 0;
};

}