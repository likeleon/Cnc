#include "cnc/stdafx.h"
#include "cnc/actor_initializer.h"
#include "cnc/actor.h"

namespace cnc {

ActorInitializer::ActorInitializer(Actor& self, const TypeDictionary& dict)
  : self_(self), dict_(dict) {
}

World& ActorInitializer::world() const {
  return self_.world();
}

}