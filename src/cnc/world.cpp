#include "cnc/stdafx.h"
#include "cnc/world.h"
#include "cnc/map.h"
#include "cnc/actor.h"
#include "cnc/type_dictionary.h"

namespace cnc {

World::World(MapUniquePtr map, OrderManager& order_manager, WorldType type)
  : map_(std::move(map)), order_manager_(order_manager), type_(type) {
  world_actor_ = CreateActor("World", TypeDictionary());
}

World::~World() {
}

ActorPtr World::CreateActor(const std::string& name, const TypeDictionary& init_dict) {
  return CreateActor(true, name, init_dict);
}

ActorPtr World::CreateActor(bool add_to_world, const std::string& name, const TypeDictionary& init_dict) {
  auto a = std::make_shared<Actor>(*this);
  a->Init(name, init_dict);
  if (add_to_world) {
    Add(a);
  }
  return a;
}

void World::Add(ActorPtr a) {
  a->set_is_in_world(true);
  actors_.emplace(a->actor_id(), a);
}

void World::Remove(ActorPtr a) {
  a->set_is_in_world(false);
  actors_.erase(a->actor_id());
}

}