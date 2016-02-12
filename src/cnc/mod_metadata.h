#pragma once

#include "cnc/field_load_info.h"
#include "cnc/content_installer.h"

namespace cnc {

struct CNC_API ModMetadata {
  static const std::map<std::string, std::string>& CandidateModPaths();
  static const std::map<std::string, ModMetadata>& AllMods();

  std::string id;
  std::string title;
  std::string description;
  std::string version;
  std::string author;
  bool hidden = false;
  ContentInstaller content;
  
  static const std::vector<FieldLoadInfo> load_info;
};


}