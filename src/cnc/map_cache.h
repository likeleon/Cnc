#pragma once

#include "cnc/cache.h"

namespace cnc {

class ModData;
class MapPreview;

class MapCache {
public:
  MapCache(const ModData& mod_data);
  ~MapCache();

  void LoadMaps();

  std::vector<const MapPreview*> Previews() const;

private:
  std::unique_ptr<MapPreview> LoadMapPreview(const std::string& uid);

  const ModData& mod_data_;
  Cache<std::string, std::unique_ptr<MapPreview>> previews_;
};

}
