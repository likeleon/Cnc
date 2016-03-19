#pragma once

#include "cnc/enum_info.h"
#include "cnc/map_ptr.h"
#include "cnc/rectangle.h"

namespace cnc {

enum class MapStatus {
  Available,
  Unavailable,
  Searching,
  DownloadAvailble,
  Downloading,
  DownloadError
};

enum class MapClassification {
  Unknown,
  System,
  User,
  Remote
};

struct MapClassificationTraits {
  using E = MapClassification;
  static const std::string pretty_name;
  static const EnumNamesType<MapClassificationTraits> names;
};

class MapCache;

class MapPreview {
public:
  MapPreview(const std::string& uid, MapCache& cache);

  void UpdateFromMap(MapUniquePtr map, MapClassification classification);

  const std::string& uid() const { return uid_; }
  const std::string& title() const { return title_; }
  const std::string& type() const { return type_; }
  const std::string& author() const { return author_; }
  int32_t player_count() const { return player_count_; }
  const Map& map() const { return *map_; }
  MapStatus status() const { return status_; }
  MapClassification classfication () const { return class_; }

private:
  MapCache& cache_;
  std::string uid_;
  std::string title_ = "Unknown map";
  std::string type_ = "Unknown";
  std::string author_ = "Unknown Author";
  int32_t player_count_ = 0;
  MapUniquePtr map_;
  MapStatus status_ = MapStatus::Unavailable;
  MapClassification class_ = MapClassification::Unknown;
};

}
