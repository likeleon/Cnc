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
      AddTrait(trait->Create());
    }

    default_visibility_ = Trait<IDefaultVisibility>();
  }
}

Actor::~Actor() {
}

void Actor::AddTrait(TypeExposablePtr trait) {
  world_.trait_dict().AddTrait(shared_from_this(), trait);
}

std::string Actor::ToString() const {
  auto name = info_->name() + " " + std::to_string(actor_id_);
  if (!is_in_world()) {
    name += " (not in world)";
  }
  return name;
}

}