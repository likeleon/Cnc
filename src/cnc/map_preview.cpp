#include "cnc/stdafx.h"
#include "cnc/map_preview.h"
#include "cnc/map.h"

namespace cnc {

const std::string MapClassificationTraits::pretty_name = "MapClassification";

const EnumNamesType<MapClassificationTraits> MapClassificationTraits::names = {
  { MapClassification::Unknown, "Unknown" },
  { MapClassification::System, "System" },
  { MapClassification::User, "User" },
  { MapClassification::Remote, "Remote" }
};

MapPreview::MapPreview(const std::string& uid, MapCache& cache)
  : cache_(cache), uid_(uid) {
}

void MapPreview::UpdateFromMap(MapPtr map, MapClassification classification) {
  map_ = map;
  title_ = map->title();
  type_ = map->type();
  author_ = map->author();
  status_ = MapStatus::Available;
  class_ = classification;
}

}