#pragma once

#include "cnc/global_mod_data.h"
#include "cnc/field_load_info.h"

namespace cnc {

class MiniYaml;

struct ContentInstaller final : public GlobalModData {
  static const std::vector<std::string> CtorArgNames;
  std::vector<FieldLoadInfo> GetLoadInfo() const;
  
  ContentInstaller();
  ContentInstaller(const MiniYaml& yaml);

  std::vector<std::type_index> Interfaces() const override;
  std::vector<std::type_index> BaseTypes() const override;

  std::vector<std::string> test_files;
  std::string package_mirror_list;
  std::string music_package_mirror_list;
  int32_t shipped_soundtracks = 0;
};

}