#pragma once

#include <unordered_map>
#include <typeindex>
#include "cnc/error.h"

namespace cnc {

class ObjectCreator {
public:
  using CreateFunc = void* (*)();

  template <typename T>
  T* CreateObject() {
    auto it = create_funcs_.find(typeid(T));
    if (it == create_funcs_.end()) {
      throw Error(MSG("type '" + type_index.name + "' not found"));
    }

    CreateFunc create_func = it.second;
    return static_cast<T*>(create_func());
  }

  template <typename T>
  void Register(CreateFunc create_func) {
    auto type_index = typeid(T);
    if (create_funcs_.find(type_index) != create_funcs_.end()) {
      throw Error(MSG("type '" + type_index.name + "' already registered"));
    }

    create_funcs_.emplace(type_index, create_func);
  }

private:
  std::unordered_map<std::type_index, CreateFunc> create_funcs_;
};

}
