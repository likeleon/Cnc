#pragma once

#include "cnc/file_system.h"
#include "cnc/field_load_info.h"

namespace cnc {

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
  static const std::vector<FieldLoadInfo> LoadInfo;
  
  explicit Map(const std::string& path);

  const std::string& path() const { return path_;  }
  const std::string& uid() const { return uid_; }
  MapVisibility visibility() const { return visibility_;  }
  const std::string& title() const { return title_; }
  const std::string& type() const { return type_; }
  const std::string& description() const { return description_; }
  const std::string& author() const { return author_; }
  const std::string& tileset() const { return tileset_; }

private:
  void AssertExists(const std::string& filename);

  IFolderPtr container_;
  std::string path_;
  
  std::string uid_;
  MapVisibility visibility_ = MapVisibility::Lobby;
  std::string title_;
  std::string type_ = "Conquest";
  std::string description_;
  std::string author_;
  std::string tileset_;
};

}
