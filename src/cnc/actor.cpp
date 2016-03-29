#include "cnc/stdafx.h"
#include "cnc/actor.h"

namespace cnc {

Actor::Actor(World& world, const std::string& name, const TypeDictionary& init_dict)
: world_(world) {
}

}