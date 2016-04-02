#pragma once

#include "cnc/itype_exposable.h"

namespace cnc {

class TypeDictionary {
public:
  void Add(const TypeExposablePtr& val);
  void Remove(ITypeExposable& val);

  template <typename T>
  bool Contains() {
    return data_.find(typeid(T)) != data_.end();
  }

  template <typename T>
  T* Get() {
    return static_cast<T*>(Get(typeid(T), true));
  }

  template <typename T>
  T* GetOrNull() {
    return static_cast<T*>(Get(typeid(T), false));
  }

  template <typename T>
  std::vector<std::shared_ptr<T>> WithInterface() {
    std::vector<std::shared_ptr<T>> ret;
    auto iter = data_.find(typeid(T));
    if (iter == data_.end()) {
      return ret;
    }
    std::transform(iter->second.begin(), iter->second.end(), std::back_inserter(ret),
                   [](const auto& p) { return std::static_pointer_cast<T>(p); });
    return ret;
  }

private:
  void InnerAdd(std::type_index t, const TypeExposablePtr& val);
  void InnerRemove(std::type_index t, ITypeExposable& val);
  ITypeExposable* Get(std::type_index t, bool throws_if_missing);

  std::map<std::type_index, std::vector<TypeExposablePtr>> data_;
};

}
