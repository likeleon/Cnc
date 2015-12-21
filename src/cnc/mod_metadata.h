#pragma once

#include "cnc/field_loader.h"

namespace cnc {

struct CNC_API ModMetadata {
  static const std::unordered_map<std::string, ModMetadata>& AllMods();

  std::string id;
  std::string title;
  std::string description;
  std::string version;
  std::string author;
  bool hidden;
  
  static const std::vector<FieldLoadInfo> load_info;
  static std::unordered_map<std::string, ModMetadata> all_mods;
  static bool all_mods_found;
};


}