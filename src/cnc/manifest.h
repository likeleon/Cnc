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
  const std::vector<std::string>& cursors() const;
  const std::vector<std::string>& chrome() const;
  const std::vector<std::string>& assemblies() const;
  const std::vector<std::string>& chrome_layout() const;
  const std::vector<std::string>& chrome_metrics() const;
  const std::map<std::string, std::string>& packages() const;
  const MiniYaml& load_screen() const;
  const std::map<std::string, std::string>& requires_mods() const;
  const std::map<std::string, std::pair<std::string, int32_t>>& fonts() const;

private:
  ModMetadata mod_;
  MiniYamlMap yaml_;
  std::vector<std::string> folders_;
  std::vector<std::string> cursors_;
  std::vector<std::string> chrome_;
  std::vector<std::string> assemblies_;
  std::vector<std::string> chrome_layout_;
  std::vector<std::string> chrome_metrics_;
  std::map<std::string, std::string> packages_;
  MiniYaml* load_screen_;
  std::map<std::string, std::string> requires_mods_;
  std::map<std::string, std::pair<std::string, int32_t>> fonts_;
};

}
