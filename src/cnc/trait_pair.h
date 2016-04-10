#pragma once

namespace cnc {

template <typename T>
struct TraitPair {
  ActorPtr actor;
  std::shared_ptr<T> trait;

  std::string ToString() const {
    return actor->info().name() + "->" + typeid(T).name();
  }
};

}
