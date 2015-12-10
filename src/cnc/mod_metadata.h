#pragma once

#include "cnc/field_loader.h"

namespace cnc {

struct ModMetadata {
  std::string id;
  std::string title;
  std::string description;
  std::string version;
  std::string author;
  bool hidden;
  
  static const std::vector<FieldLoadInfo> load_info;
};

const std::unordered_map<std::string, ModMetadata>& AllMods();

}