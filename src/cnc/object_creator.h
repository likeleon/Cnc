#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "cnc/error.h"

namespace cnc {

template <typename T>
T* New() {
  return new T;
}

class ObjectCreator {
public:
  template <typename T>
  using CreateFunc = T* (*)();

  template <typename T>
  std::unique_ptr<T> CreateObject(const std::string& type_name) {
    auto it = create_funcs_.find(type_name);
    if (it == create_funcs_.end()) {
      throw Error(MSG("type '" + type_name + "' not found"));
    }

    T* obj = static_cast<T*>(it->second());
    return std::unique_ptr<T>(obj);
  }

  template <typename T>
  void Register(const std::string& type_name, CreateFunc<T> create_func) {
    if (create_funcs_.find(type_name) != create_funcs_.end()) {
      throw Error(MSG("type '" + type_name + "' already registered"));
    }

    create_funcs_.emplace(type_name, reinterpret_cast<CreateFunc<void>>(create_func));
  }

private:
  std::unordered_map<std::string, CreateFunc<void>> create_funcs_;
};

}
