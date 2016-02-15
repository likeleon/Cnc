#pragma once

#include "cnc/error.h"
#include "cnc/any.h"

namespace cnc {

class CNC_API ObjectCreator {
public:
  template <typename T>
  std::unique_ptr<T> CreateObject(const std::string& type_name, const std::map<std::string, Any>& args) {
    auto it = create_funcs_.find(type_name);
    if (it == create_funcs_.end()) {
      throw Error(MSG("type '" + type_name + "' not found"));
    }

    // TODO: Compare args.keys between T::CtorArgNames, throw with missing args.

    T* obj = static_cast<T*>(it->second(args));
    return std::unique_ptr<T>(obj);
  }

  template <typename T, typename... CtorArgs>
  void Register(const std::string& type_name) {
    if (create_funcs_.find(type_name) != create_funcs_.end()) {
      throw Error(MSG("type '" + type_name + "' already registered"));
    }

    create_funcs_.emplace(type_name, &Ctor<T, CtorArgs...>::Create);
  }

  bool TypeRegistered(const std::string& type_name) {
    return (create_funcs_.find(type_name) != create_funcs_.end());
  }

private:
  using CreateFunc = std::function<void* (const std::map<std::string, Any>&)>;
  
  template <typename T, typename... CtorArgs>
  class Ctor {
  public:
    using RuntimeArgs = std::map<std::string, Any>;

    static T* Create(const RuntimeArgs& a) {
      return CreateInternal(a, BuildIndices<sizeof...(CtorArgs)>{});
    }

  private:
    template <std::size_t... Indices>
    struct Indices {};

    template <std::size_t N, std::size_t... Is>
    struct BuildIndices : BuildIndices<N - 1, N - 1, Is...> {};

    template <std::size_t... Is>
    struct BuildIndices<0, Is...> : Indices<Is...> {};

    template <std::size_t... Is>
    static T* CreateInternal(const RuntimeArgs& a, Indices<Is...>) {
      (a);
      return new T(AnyCast<CtorArgs>(a.at(T::CtorArgNames[Is]))...);
    }
  };

  std::unordered_map<std::string, CreateFunc> create_funcs_;
};

}
