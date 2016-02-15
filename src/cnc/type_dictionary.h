#pragma once

#include "cnc/type_exposable.h"

namespace cnc {

class TypeDictionary {
public:
  void Add(const TypeExposablePtr& val);
  void Remove(TypeExposable& val);

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
  std::vector<T*> WithInterface() {
    std::vector<T*> objs;
    auto iter = data_.find(t);
    if (iter != data_.end()) {
      objs = iter->second;
    }
    return objs;
  }

private:
  void InnerAdd(std::type_index t, const TypeExposablePtr& val);
  void InnerRemove(std::type_index t, TypeExposable& val);
  TypeExposable* Get(std::type_index t, bool throws_if_missing);

  std::map<std::type_index, std::vector<TypeExposablePtr>> data_;
};

}
