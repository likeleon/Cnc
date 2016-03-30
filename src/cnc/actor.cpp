#include "cnc/stdafx.h"
#include "cnc/actor.h"
#include "cnc/world.h"
#include "cnc/string_utils.h"
#include "cnc/map.h"
#include "cnc/ruleset.h"

namespace cnc {

Actor::Actor(World& world, std::string name, const TypeDictionary& init_dict)
  : world_(world), actor_id_(world.NextAID()) {
  if (!name.empty()) {
    name = StringUtils::ToLower(name);

    auto iter = world.map().rules().actors().find(name);
  }
}

}