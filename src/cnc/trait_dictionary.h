#pragma once

#include "cnc/container_utils.h"
#include "cnc/actor.h"
#include "cnc/actor_info.h"

namespace cnc {

class Actor;

class TraitDictionary {
public:
  void AddTrait(ActorPtr actor, TypeExposablePtr val);

  template <typename T>
  std::shared_ptr<T> Get(const Actor& actor);

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

  template <typename T>
  class TraitContainer : public ITraitContainer {
  public:
    using Ptr = std::unique_ptr<TraitContainer>;

    void Add(ActorPtr actor, TypeExposablePtr trait) override;
    void RemoveActor(uint32_t actor) override;
    int32_t queries() const override { return queries_; }
    void set_queries(int32_t value) { queries_ = value; }
    
    std::shared_ptr<T> Get(uint32_t actor);
    std::shared_ptr<T> GetOrDefault(uint32_t actor);

  private:
    int32_t ActorsBinarySearchMany(uint32_t search_for) const;

    int32_t queries_ = 0;
    std::vector<ActorPtr> actors_;
    std::vector<std::shared_ptr<T>> traits_;
  };

  static void CheckDestroyed(const Actor& actor);

  template <typename T>
  static ITraitContainerUniquePtr CreateTraitContainer();

  ITraitContainer& InnerGet(std::type_index t);

  template <typename T>
  TraitContainer<T>& InnerGet();

  std::map<std::type_index, ITraitContainerUniquePtr> traits_;
};

// TraitContainer<T>
template <typename T>
void TraitDictionary::TraitContainer<T>::Add(ActorPtr actor, TypeExposablePtr trait) {
  auto insert_index = ActorsBinarySearchMany(actor->actor_id() + 1);
  actors_.insert(actors_.begin() + insert_index, actor);
  traits_.insert(traits_.begin() + insert_index, std::static_pointer_cast<T>(trait));
}

template <typename T>
void TraitDictionary::TraitContainer<T>::RemoveActor(uint32_t actor) {
  auto start_index = ActorsBinarySearchMany(actor);
  if (start_index >= actors_.size() || actors_[start_index]->actor_id() != actor) {
    return;
  }
  auto end_index = start_index + 1;
  while (end_index < actors_.size() && actors_[end_index]->actor_id() == actor) {
    ++end_index;
  }
  actors_.erase(actors_.begin() + start_index, actors_.begin() + end_index);
  traits_.erase(traits_.begin() + start_index, traits_.begin() + end_index);
}

template <typename T>
std::shared_ptr<T> TraitDictionary::TraitContainer<T>::Get(uint32_t actor) {
  auto result = GetOrDefault(actor);
  if (result == nullptr) {
    throw Error(MSG("Actor does not have trait of type '" + std::string(typeid(T).name()) + "'"));
  }
  return result;
}

template <typename T>
std::shared_ptr<T> TraitDictionary::TraitContainer<T>::GetOrDefault(uint32_t actor) {
  ++queries_;
  auto index = ActorsBinarySearchMany(actor);
  if (index >= actors_.size() || actors_[index]->actor_id() != actor) {
    return nullptr;
  } else if (index + 1 < actors_.size() && actors_[index + 1]->actor_id() == actor) {
    std::ostringstream oss;
    oss << "Actor " << actors_[index]->info().name() << " has multiple traits of type '" << typeid(T).name << "'";
    throw Error(MSG(oss.str()));
  } else {
    return traits_[index];
  }
}

template <typename T>
int32_t TraitDictionary::TraitContainer<T>::ActorsBinarySearchMany(uint32_t search_for) const {
  auto low = std::lower_bound(actors_.begin(), actors_.end(), search_for,
                              [&](const auto& a) { return a->actor_id() < search_for; });
  return low - actors_.begin();
}

// TraitDictionary
template <typename T>
std::shared_ptr<T> TraitDictionary::Get(const Actor& actor) {
  CheckDestroyed(actor);
  return InnerGet<T>().Get(actor.actor_id());
}

template <typename T>
TraitDictionary::ITraitContainerUniquePtr TraitDictionary::CreateTraitContainer() {
  return std::make_unique<TraitContainer<T>>();
}

TraitDictionary::ITraitContainer& TraitDictionary::InnerGet(std::type_index t) {
  return GetOrAdd(traits_, t, CreateTraitContainer<T>);
}

template <typename T>
TraitDictionary::TraitContainer<T>& TraitDictionary::InnerGet() {
  return reinterpret_cast<TraitContainer<T>&>(*c);
}

}
