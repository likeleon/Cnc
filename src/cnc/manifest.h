#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/mod_metadata.h"
#include "cnc/type_dictionary.h"

namespace cnc {

class ObjectCreator;

class CNC_API Manifest {
public:
  Manifest(const std::string& mod_id, const std::string& mod_path = "");
  
  static const std::unordered_map<std::string, Manifest>& AllMods();

  void LoadCustomData(ObjectCreator& oc);

  template <typename T>
  T& Get() {
    static_assert(std::is_base_of<GlobalModData, T>(), "T must inherit from GlobalModData");
    
    T* module = modules_.GetOrNull<T>();
    if (module == nullptr) {
      auto ptr = std::make_shared<T>();
      modules_.Add(ptr);
      module = ptr.get();
    }

    return *module;
  }

  const ModMetadata& mod() const { return mod_; }
  const std::vector<std::string>& folders() const { return folders_; }
  const std::vector<std::string>& rules() const { return rules_; }
  const std::vector<std::string>& cursors() const { return cursors_; }
  const std::vector<std::string>& chrome() const { return chrome_; }
  const std::vector<std::string>& assemblies() const { return assemblies_; }
  const std::vector<std::string>& chrome_layout() const { return chrome_layout_; }
  const std::vector<std::string>& tile_sets() const { return tile_sets_; }
  const std::vector<std::string>& chrome_metrics() const { return chrome_metrics_; }
  const std::map<std::string, std::string>& packages() const { return packages_; }
  const std::map<std::string, std::string>& map_folders() const { return map_folders_; }
  const MiniYaml& load_screen() const { return *load_screen_; }
  const std::map<std::string, std::string>& requires_mods() const { return requires_mods_; }
  const std::map<std::string, std::pair<std::string, int32_t>>& fonts() const { return fonts_; }
  const std::vector<std::string>& sprite_formats() const { return sprite_formats_; }

private:
  ModMetadata mod_;
  std::vector<std::string> folders_;
  std::vector<std::string> rules_;
  std::vector<std::string> cursors_;
  std::vector<std::string> chrome_;
  std::vector<std::string> assemblies_;
  std::vector<std::string> chrome_layout_;
  std::vector<std::string> tile_sets_;
  std::vector<std::string> chrome_metrics_;
  std::map<std::string, std::string> packages_;
  std::map<std::string, std::string> map_folders_;
  MiniYaml* load_screen_;
  std::map<std::string, std::string> requires_mods_;
  std::map<std::string, std::pair<std::string, int32_t>> fonts_;
  std::vector<std::string> sprite_formats_;
  TypeDictionary modules_;
  MiniYamlMap yaml_;
};

}
