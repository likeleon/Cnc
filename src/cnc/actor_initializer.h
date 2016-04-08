#pragma once

#include "cnc/type_dictionary.h"

namespace cnc {

class Actor;
class World;

class CNC_API ActorInitializer {
public:
  ActorInitializer(Actor& self, const TypeDictionary& dict);

  Actor& self() const { return self_; }
  World& world() const;

  template <typename T>
  T* Get() const {
    return dict_.Get<T>();
  }

  template <typename T, typename U>
  U* Get() const {
    return dict_.Get<T>().Value(world());
  }

  template <typename T>
  bool Contains() const {
    return dict_.Contains<T>();
  }

  ActorInitializer(const ActorInitializer&) = delete;
  ActorInitializer& operator=(const ActorInitializer&) = delete;
  ActorInitializer& operator=(ActorInitializer&&) = delete;

private:
  Actor& self_;
  const TypeDictionary& dict_;
};

}
