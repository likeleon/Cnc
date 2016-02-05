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
  bool hidden = false;
  
  static const std::vector<FieldLoadInfo> load_info;
};


}