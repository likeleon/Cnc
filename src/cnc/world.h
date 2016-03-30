#pragma once

#include "cnc/map_ptr.h"
#include "cnc/actor_ptr.h"

namespace cnc {

class OrderManager;
class TypeDictionary;

enum class WorldType {
  Regular,
  Shellmap
};

class World {
public:
  World(MapUniquePtr map, OrderManager& order_manager, WorldType type);
  ~World();

  Map& map() { return *map_; }

  ActorPtr CreateActor(const std::string& name, const TypeDictionary& init_dict);
  ActorPtr CreateActor(bool add_to_world, const std::string& name, const TypeDictionary& init_dict);

  void Add(ActorPtr a);
  void Remove(ActorPtr a);

  World(const World& other) = delete;
  World& operator=(const World& other) = delete;

private:
  std::map<uint32_t, ActorPtr> actors_;
  ActorPtr world_actor_;
  MapUniquePtr map_;
  OrderManager& order_manager_;
  WorldType type_;
};

};
