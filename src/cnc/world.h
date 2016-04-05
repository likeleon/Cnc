#pragma once

#include "cnc/map_ptr.h"
#include "cnc/actor_ptr.h"
#include "cnc/trait_dictionary.h"

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

  TraitDictionary& trait_dict() { return trait_dict_; }
  Actor& world_actor() { return *world_actor_; }
  Map& map() { return *map_; }

  ActorPtr CreateActor(const std::string& name, const TypeDictionary& init_dict);
  ActorPtr CreateActor(bool add_to_world, const std::string& name, const TypeDictionary& init_dict);

  void Add(ActorPtr a);
  void Remove(ActorPtr a);

  uint32_t NextAID() { return next_aid_++; }

  World(const World& other) = delete;
  World& operator=(const World& other) = delete;

private:
  TraitDictionary trait_dict_;
  std::map<uint32_t, ActorPtr> actors_;
  ActorPtr world_actor_;
  MapUniquePtr map_;
  OrderManager& order_manager_;
  WorldType type_;
  uint32_t next_aid_ = 0;
};

};
