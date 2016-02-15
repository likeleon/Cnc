#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/mod_metadata.h"
#include "cnc/type_dictionary.h"
#include "cnc/game.h"
#include "cnc/object_creator.h"

namespace cnc {

class ObjectCreator;

class CNC_API Manifest {
public:
  Manifest(const std::string& mod_id, const std::string& mod_path = "");
  
  static const std::unordered_map<std::string, Manifest>& AllMods();

  void LoadCustomData(ObjectCreator& oc);

  template <typename T>
  T& Get(const std::string& name) {
    static_assert(std::is_base_of<GlobalModData, T>(), "T must inherit from GlobalModData");
    
    T* module = modules_.GetOrNull<T>();
    if (module == nullptr) {
      module = Game::mod_data()->object_creator().CreateObject<T>(name, {}).release();
      modules_.Add(std::shared_ptr<T>(module));
    }
    
    return *module;
  }

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
  const std::vector<std::string>& sprite_formats() const;

private:
  ModMetadata mod_;
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
  std::vector<std::string> sprite_formats_;
  TypeDictionary modules_;
  MiniYamlMap yaml_;
};

}
