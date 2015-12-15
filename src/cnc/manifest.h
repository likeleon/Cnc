#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/mod_metadata.h"

namespace cnc {

class Manifest {
public:
  Manifest(const std::string& mod);
  
  static const std::unordered_map<std::string, Manifest>& AllMods();

  const std::vector<std::string>& folders() const;
  const std::vector<std::string>& assemblies() const;

private:
  static std::vector<std::string> YamlList(const std::unordered_map<std::string, MiniYaml>& yaml,
                                           const std::string& key,
                                           bool parse_paths = false);

  ModMetadata mod_;
  std::unordered_map<std::string, MiniYaml> yaml_;
  std::vector<std::string> folders_;
  std::vector<std::string> assemblies_;
  MiniYaml* load_screen_;
};

}
