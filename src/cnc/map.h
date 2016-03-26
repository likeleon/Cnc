#pragma once

#include "cnc/field_load_info.h"
#include "cnc/ifolder.h"
#include "cnc/enum_info.h"
#include "cnc/mini_yaml.h"
#include "cnc/lazy.h"
#include "cnc/action.h"

namespace cnc {

class Ruleset;

enum MapVisibility {
  Lobby = 1,
  Shellmap = 2,
  MissionSelector = 4
};

struct MapVisibilityTraits {
  using E = MapVisibility;
  static const std::string pretty_name;
  static const EnumNamesType<MapVisibilityTraits> names;
};

class Map {
public:
  std::vector<FieldLoadInfo> GetLoadInfo() const;
  
  explicit Map(const std::string& path);
  ~Map();

  const std::string& path() const { return path_;  }
  const std::string& uid() const { return uid_; }
  MapVisibility visibility() const { return visibility_;  }
  const std::string& title() const { return title_; }
  const std::string& type() const { return type_; }
  const std::string& description() const { return description_; }
  const std::string& author() const { return author_; }
  const std::string& tileset() const { return tileset_; }
  bool invalid_custom_rules() const { return invalid_custom_rules_; }

  // Yaml map data
  const MiniYamlNodes& rule_definitions() const { return rule_definitions_; }

  // Binary map data
  Ruleset& rules() { return *rules_(); }

private:
  void AssertExists(const std::string& filename);
  std::string ComputeHash() const;
  void PostInit();
  std::shared_ptr<Ruleset> LoadRuleset();

  static const int MinimumSupportedMapFormat = 6;

  IFolderPtr container_;
  std::string path_;
  
  std::string uid_;
  int32_t map_format_;
  MapVisibility visibility_ = MapVisibility::Lobby;
  std::string title_;
  std::string type_ = "Conquest";
  std::string description_;
  std::string author_;
  std::string tileset_;

  MiniYamlNodes rule_definitions_;
  
  Lazy<Func<std::shared_ptr<Ruleset>>> rules_;
  bool invalid_custom_rules_ = false;
};

}
