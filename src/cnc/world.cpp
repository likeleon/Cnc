#include "cnc/stdafx.h"
#include "cnc/world.h"
#include "cnc/map.h"

namespace cnc {

World::World(MapUniquePtr map, OrderManager& order_manager, WorldType type)
  : map_(std::move(map)), order_manager_(order_manager), type_(type) {
}

World::~World() {
}

}