#pragma once

#include "cnc/map_ptr.h"

namespace cnc {

class OrderManager;

enum class WorldType {
  Regular,
  Shellmap
};

class World {
public:
  World(MapUniquePtr map, OrderManager& order_manager, WorldType type);
  ~World();

  Map& map() { return *map_; }

  World(const World& other) = delete;
  World& operator=(const World& other) = delete;

private:
  MapUniquePtr map_;
  OrderManager& order_manager_;
  WorldType type_;
};

};
