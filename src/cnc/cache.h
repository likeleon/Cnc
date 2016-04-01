#pragma once

#include "cnc/container_utils.h"

namespace cnc {

template <typename Key, typename T, typename Hasher = std::hash<Key>>
class Cache {
public:
  using Loader = std::function<T(const Key&)>;
  using Container = std::unordered_map<Key, T, Hasher>;

  Cache(const Loader& loader)
    : loader_(loader) {
  }

  T& operator[](const Key& key) {
    return GetOrAdd(cache_, key, loader_);
  }

  void Clear() {
    cache_.clear();
  }

  typename Container::iterator begin() {
    return cache_.begin();
  }

  typename Container::iterator end() {
    return cache_.end();
  }

  typename Container::const_iterator begin() const {
    return cache_.begin();
  }

  typename Container::const_iterator end() const {
    return cache_.end();
  }

private:
  Loader loader_;
  Container cache_;
};

}
