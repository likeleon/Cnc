#pragma once

#include "cnc/actor_info_ptr.h"
#include "cnc/world.h"

namespace cnc {

class World;
class TypeDictionary;
class IDefaultVisibility;

class Actor : public std::enable_shared_from_this<Actor> {
public:
  explicit Actor(World& world);
  ~Actor();

  void Init(std::string name, const TypeDictionary& init_dict);

  uint32_t actor_id() const { return actor_id_; }
  World& world() { return world_; }
  void set_is_in_world(bool value) { is_in_world_ = value; }
  bool is_in_world() const { return is_in_world_; }
  bool disposed() const { return disposed_; }
  const ActorInfo& info() const { return *info_; }

  void AddTrait(ITraitPtr trait);
  
  template <typename T>
  std::shared_ptr<T> Trait();

  std::string ToString() const;

  Actor(const Actor& other) = delete;
  Actor& operator=(const Actor& other) = delete;
  Actor& operator=(Actor&& other) = delete;

private:
  World& world_;
  uint32_t actor_id_ = 0;
  bool is_in_world_ = false;
  bool disposed_ = false;
  ActorInfoPtr info_ = nullptr;
  std::shared_ptr<IDefaultVisibility> default_visibility_;
};

template <typename T>
std::shared_ptr<T> Actor::Trait() {
  return world_.trait_dict().Get<T>(*this);
}

}