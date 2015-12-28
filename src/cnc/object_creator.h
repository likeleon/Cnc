#pragma once

#include "cnc/error.h"
#include "cnc/any.h"

namespace cnc {

template <typename T>
T* New(const std::map<std::string, Any>& args) {
  return new T(args);
}

class CNC_API ObjectCreator {
public:
  template <typename T>
  using CreateFunc = T* (*)(const std::map<std::string, Any>&);

  template <typename T>
  std::unique_ptr<T> CreateObject(const std::string& type_name, const std::map<std::string, Any>& args) {
    auto it = create_funcs_.find(type_name);
    if (it == create_funcs_.end()) {
      throw Error(MSG("type '" + type_name + "' not found"));
    }

    T* obj = static_cast<T*>(it->second(args));
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
