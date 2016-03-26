#pragma once

#include "cnc/error.h"
#include "cnc/any.h"

namespace cnc {

class CNC_API ObjectCreator {
public:
  template <typename T>
  std::shared_ptr<T> CreateObject(const std::string& type_name) {
    return CreateObject<T>(type_name, {});
  }

  template <typename T>
  std::shared_ptr<T> CreateObject(const std::string& type_name, const std::map<std::string, Any>& args) {
    auto it = create_funcs_.find(type_name);
    if (it == create_funcs_.end()) {
      throw Error(MSG("type '" + type_name + "' not found"));
    }

    // TODO: Compare args.keys between T::CtorArgNames, throw with missing args.

    return std::static_pointer_cast<T>(it->second(args));
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
  using CreateFunc = std::function<std::shared_ptr<void> (const std::map<std::string, Any>&)>;
  
  template <typename T, typename... CtorArgs>
  class Ctor {
  public:
    using RuntimeArgs = std::map<std::string, Any>;

    static std::shared_ptr<T> Create(const RuntimeArgs& a) {
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
    static std::shared_ptr<T> CreateInternal(const RuntimeArgs& a, Indices<Is...>) {
      (a);
      return std::make_shared<T>(AnyCast<CtorArgs>(a.at(T::CtorArgNames[Is]))...);
    }
  };

  std::unordered_map<std::string, CreateFunc> create_funcs_;
};

}
