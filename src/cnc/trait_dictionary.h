#pragma once

#include "cnc/actor_ptr.h"
#include "cnc/actor_info.h"

namespace cnc {

class Actor;

class TraitDictionary {
public:
  void AddTrait(ActorPtr actor, TypeExposablePtr val);

  template <typename T>
  std::shared_ptr<T> Get(const Actor& actor);

  template <typename T>
  std::shared_ptr<T> GetOrDefault(const Actor& actor);

private:
  class ITraitContainer {
  public:
    virtual ~ITraitContainer() {}
    virtual void Add(ActorPtr actor, TypeExposablePtr trait) = 0;
    virtual void RemoveActor(uint32_t actor) = 0;
    virtual int32_t queries() const = 0;
    virtual void set_queries(int32_t value) = 0;
  };

  using ITraitContainerUniquePtr = std::unique_ptr<ITraitContainer>;

  class TraitContainer : public ITraitContainer {
  public:
    using Ptr = std::unique_ptr<TraitContainer>;

    explicit TraitContainer(std::type_index type_index);

    void Add(ActorPtr actor, TypeExposablePtr trait) override;
    void RemoveActor(uint32_t actor) override;
    int32_t queries() const override { return queries_; }
    void set_queries(int32_t value) { queries_ = value; }
    
    TypeExposablePtr Get(uint32_t actor);
    TypeExposablePtr GetOrDefault(uint32_t actor);

  private:
    size_t ActorsBinarySearchMany(uint32_t search_for) const;

    std::type_index type_index_;
    int32_t queries_ = 0;
    std::vector<ActorPtr> actors_;
    std::vector<TypeExposablePtr> traits_;
  };

  static void CheckDestroyed(const Actor& actor);
  static ITraitContainerUniquePtr CreateTraitContainer(std::type_index t);

  void InnerAdd(ActorPtr actor, std::type_index t, TypeExposablePtr val);

  ITraitContainer& InnerGet(std::type_index t);

  template <typename T>
  TraitContainer& InnerGet();

  std::map<std::type_index, ITraitContainerUniquePtr> traits_;
};

template <typename T>
std::shared_ptr<T> TraitDictionary::Get(const Actor& actor) {
  CheckDestroyed(actor);
  return std::static_pointer_cast<T>(InnerGet<T>().Get(actor.actor_id()));
}

template <typename T>
std::shared_ptr<T> TraitDictionary::GetOrDefault(const Actor& actor) {
  CheckDestroyed(actor);
  return std::static_pointer_cast<T>(InnerGet<T>().GetOrDefault(actor.actor_id()));
}

template <typename T>
TraitDictionary::TraitContainer& TraitDictionary::InnerGet() {
  return reinterpret_cast<TraitContainer&>(InnerGet(typeid(T)));
}

}
