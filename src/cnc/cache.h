#pragma once

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
    auto iter = cache_.find(key);
    if (iter == cache_.end()) {
      iter = cache_.emplace(key, loader_(key)).first;
    }
    return iter->second;
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
