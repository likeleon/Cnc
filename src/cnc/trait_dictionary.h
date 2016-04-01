#pragma once

#include "cnc/container_utils.h"

namespace cnc {

class Actor;

class TraitDictionary {
public:
  template <typename T>
  std::shared_ptr<T> Get(const Actor& actor);

private:
  class ITraitContainer {
  public:
    virtual ~ITraitContainer() {}
    virtual void Add(const Actor& actor, TypeExposablePtr trait) = 0;
    virtual void RemoveActor(const Actor& actor) = 0;
    virtual int32_t queries() const = 0;
    virtual void set_queries(int32_t value) = 0;
  };

  using ITraitContainerUniquePtr = std::unique_ptr<ITraitContainer>;

  template <typename T>
  class TraitContainer : public ITraitContainer {
  public:
    using Ptr = std::unique_ptr<TraitContainer>;
    
    std::shared_ptr<T> Get(uint32_t actor);
    std::shared_ptr<T> GetOrDefault(uint32_t actor);
  };

  static void CheckDestroyed(const Actor& actor);

  template <typename T>
  static ITraitContainerUniquePtr CreateTraitContainer();

  template <typename T>
  TraitContainer<T>& InnerGet();

  std::map<std::type_index, ITraitContainerUniquePtr> traits_;
};

template <typename T>
std::shared_ptr<T> TraitDictionary::TraitContainer<T>::Get(uint32_t actor) {
  auto result = GetOrDefault(actor);
  if (result == nullptr) {
    throw Error(MSG("Actor does not have trait of type '" + std::string(typeid(T).name()) + "'"));
  }
  return result;
}

template <typename T>
std::shared_ptr<T> TraitDictionary::Get(const Actor& actor) {
  CheckDestroyed(actor);
  return InnerGet<T>().Get(actor.actor_id());
}

template <typename T>
TraitDictionary::ITraitContainerUniquePtr TraitDictionary::CreateTraitContainer() {
  return std::make_unique<TraitContainer<T>>();
}

template <typename T>
TraitDictionary::TraitContainer<T>& TraitDictionary::InnerGet() {
  auto c = GetOrAdd(traits_, typeid(T), CreateTraitContainer<T>);
  return reinterpret_cast<TraitContainer<T>&>(*c);
}

}
