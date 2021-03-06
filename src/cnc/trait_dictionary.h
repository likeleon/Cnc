#pragma once

#include "cnc/actor_info.h"
#include "cnc/trait_pair.h"

namespace cnc {

class Actor;

class TraitDictionary {
public:
  void AddTrait(ActorPtr actor, ITraitPtr val);

  template <typename T>
  std::shared_ptr<T> Get(const Actor& actor);

  template <typename T>
  std::shared_ptr<T> GetOrDefault(const Actor& actor);

  template <typename T>
  std::vector<TraitPair<T>> ActorsWithTrait();

private:
  class ITraitContainer {
  public:
    virtual ~ITraitContainer() {}
    virtual void Add(ActorPtr actor, ITraitPtr trait) = 0;
    virtual void RemoveActor(uint32_t actor) = 0;
    virtual int32_t queries() const = 0;
    virtual void set_queries(int32_t value) = 0;
  };

  using ITraitContainerUniquePtr = std::unique_ptr<ITraitContainer>;

  class TraitContainer : public ITraitContainer {
  public:
    using Ptr = std::unique_ptr<TraitContainer>;

    explicit TraitContainer(std::type_index type_index);

    void Add(ActorPtr actor, ITraitPtr trait) override;
    void RemoveActor(uint32_t actor) override;
    int32_t queries() const override { return queries_; }
    void set_queries(int32_t value) { queries_ = value; }
    
    template <typename T>
    std::shared_ptr<T> Get(uint32_t actor);
    
    template <typename T>
    std::shared_ptr<T> GetOrDefault(uint32_t actor);

    template <typename T>
    std::vector<TraitPair<T>> All();

  private:
    size_t ActorsBinarySearchMany(uint32_t search_for) const;

    template <typename T>
    std::shared_ptr<T> CastTrait(ITraitPtr trait);

    std::type_index type_index_;
    int32_t queries_ = 0;
    std::vector<ActorPtr> actors_;
    std::vector<ITraitPtr> traits_;
  };

  static void CheckDestroyed(const Actor& actor);
  static ITraitContainerUniquePtr CreateTraitContainer(std::type_index t);

  void InnerAdd(ActorPtr actor, std::type_index t, ITraitPtr val);

  ITraitContainer& InnerGet(std::type_index t);

  template <typename T>
  TraitContainer& InnerGet();

  std::map<std::type_index, ITraitContainerUniquePtr> traits_;
};

template <typename T>
std::shared_ptr<T> TraitDictionary::Get(const Actor& actor) {
  CheckDestroyed(actor);
  return InnerGet<T>().Get<T>(actor.actor_id());
}

template <typename T>
std::shared_ptr<T> TraitDictionary::GetOrDefault(const Actor& actor) {
  CheckDestroyed(actor);
  return InnerGet<T>().GetOrDefault<T>(actor.actor_id());
}

template <typename T>
std::vector<TraitPair<T>> TraitDictionary::ActorsWithTrait() {
  return InnerGet<T>().All<T>();
}

template <typename T>
TraitDictionary::TraitContainer& TraitDictionary::InnerGet() {
  return reinterpret_cast<TraitContainer&>(InnerGet(typeid(T)));
}

// TraitDictionary::TraitContainer
template <typename T>
std::shared_ptr<T> TraitDictionary::TraitContainer::Get(uint32_t actor) {
  auto result = GetOrDefault<T>(actor);
  if (result == nullptr) {
    throw Error(MSG("Actor does not have trait of type '" + std::string(type_index_.name()) + "'"));
  }
  return result;
}

template <typename T>
std::shared_ptr<T> TraitDictionary::TraitContainer::GetOrDefault(uint32_t actor) {
  ++queries_;
  auto index = ActorsBinarySearchMany(actor);
  if (index >= actors_.size() || actors_[index]->actor_id() != actor) {
    return nullptr;
  } else if (index + 1 < actors_.size() && actors_[index + 1]->actor_id() == actor) {
    std::ostringstream oss;
    oss << "Actor " << actors_[index]->info().name() << " has multiple traits of type '" << type_index_.name() << "'";
    throw Error(MSG(oss.str()));
  } else {
    return CastTrait<T>(traits_[index]);
  }
}

template <typename T>
std::shared_ptr<T> TraitDictionary::TraitContainer::CastTrait(ITraitPtr trait) {
  return std::dynamic_pointer_cast<T>(trait);
}

template <typename T>
std::vector<TraitPair<T>> TraitDictionary::TraitContainer::All() {
  std::vector<TraitPair<T>> ret;
  ret.reserve(actors_.size());
  for (size_t i = 0; i < actors_.size(); ++i) {
    ret.emplace_back(TraitPair<T>{ actors_[i], CastTrait<T>(traits_[i]) });
  }
  return ret;
}

}
