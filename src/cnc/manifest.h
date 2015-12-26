#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/mod_metadata.h"

namespace cnc {

class CNC_API Manifest {
public:
  Manifest(const std::string& mod);
  
  static const std::unordered_map<std::string, Manifest>& AllMods();

  const ModMetadata& mod() const;
  const std::vector<std::string>& folders() const;
  const std::vector<std::string>& assemblies() const;
  const std::vector<std::string>& chrome_layout() const;
  const MiniYaml& load_screen() const;

private:
  static std::vector<std::string> YamlList(const std::unordered_map<std::string, MiniYaml>& yaml,
                                           const std::string& key,
                                           bool parse_paths = false);

  ModMetadata mod_;
  std::unordered_map<std::string, MiniYaml> yaml_;
  std::vector<std::string> folders_;
  std::vector<std::string> assemblies_;
  std::vector<std::string> chrome_layout_;
  MiniYaml* load_screen_;
};

}
