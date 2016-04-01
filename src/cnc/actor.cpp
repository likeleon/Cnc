#include "cnc/stdafx.h"
#include "cnc/actor.h"
#include "cnc/world.h"
#include "cnc/string_utils.h"
#include "cnc/map.h"
#include "cnc/ruleset.h"
#include "cnc/container_utils.h"
#include "cnc/actor_info.h"
#include "cnc/traits_interfaces.h"

namespace cnc {

Actor::Actor(World& world, std::string name, const TypeDictionary& /*init_dict*/)
  : world_(world), actor_id_(world.NextAID()) {
  if (!name.empty()) {
    name = StringUtils::ToLower(name);

    if (!ContainsKey(world.map().rules().actors(), name)) {
      throw Error(MSG("No rules definition for unit " + name));
    }

    info_ = world.map().rules().actors().at(name);
    for (const auto& trait : info_->TraitsInConstructionOrder()) {
      auto trait_obj = trait->Create();
      (trait_obj);
    }
  }
}

}