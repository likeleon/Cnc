#include "cnc/stdafx.h"
#include "cnc/trait_dictionary.h"
#include "cnc/container_utils.h"
#include "cnc/actor.h"
#include "cnc/error.h"

namespace cnc {

// TraitDictionary
void TraitDictionary::AddTrait(ActorPtr actor, TypeExposablePtr val) {
  for (const auto& tt : val->Types()) {
    InnerAdd(actor, tt, val);
  }
}

void TraitDictionary::InnerAdd(ActorPtr actor, std::type_index t, TypeExposablePtr val) {
  InnerGet(t).Add(actor, val);
}

void TraitDictionary::CheckDestroyed(const Actor& actor) {
  if (actor.disposed()) {
    throw Error(MSG("Attempted to get trait from destroyed object (" + actor.ToString() + ")"));
  }
}

TraitDictionary::ITraitContainerUniquePtr TraitDictionary::CreateTraitContainer(std::type_index t) {
  return std::make_unique<TraitContainer>(t);
}

TraitDictionary::ITraitContainer& TraitDictionary::InnerGet(std::type_index t) {
  return *GetOrAdd(traits_, t, CreateTraitContainer);
}

// TraitDictionary::TraitContainer
TraitDictionary::TraitContainer::TraitContainer(std::type_index type_index)
  : type_index_(type_index) {
}

void TraitDictionary::TraitContainer::Add(ActorPtr actor, TypeExposablePtr trait) {
  auto insert_index = ActorsBinarySearchMany(actor->actor_id() + 1);
  actors_.insert(actors_.begin() + insert_index, actor);
  traits_.insert(traits_.begin() + insert_index, trait);
}

void TraitDictionary::TraitContainer::RemoveActor(uint32_t actor) {
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

TypeExposablePtr TraitDictionary::TraitContainer::Get(uint32_t actor) {
  auto result = GetOrDefault(actor);
  if (result == nullptr) {
    throw Error(MSG("Actor does not have trait of type '" + std::string(type_index_.name()) + "'"));
  }
  return result;
}

TypeExposablePtr TraitDictionary::TraitContainer::GetOrDefault(uint32_t actor) {
  ++queries_;
  auto index = ActorsBinarySearchMany(actor);
  if (index >= actors_.size() || actors_[index]->actor_id() != actor) {
    return nullptr;
  } else if (index + 1 < actors_.size() && actors_[index + 1]->actor_id() == actor) {
    std::ostringstream oss;
    oss << "Actor " << actors_[index]->info().name() << " has multiple traits of type '" << type_index_.name() << "'"; 
    throw Error(MSG(oss.str()));
  } else {
    return traits_[index];
  }
}

size_t TraitDictionary::TraitContainer::ActorsBinarySearchMany(uint32_t search_for) const {
  auto low = std::lower_bound(actors_.begin(), actors_.end(), search_for,
                              [&](const auto& a, uint32_t b) { return a->actor_id() < b; });
  return low - actors_.begin();
}


}