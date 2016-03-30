#pragma once

namespace cnc {

class World;
class TypeDictionary;

class Actor {
public:
  Actor(World& world, std::string name, const TypeDictionary& init_dict);

  uint32_t actor_id() const { return actor_id_; }
  void set_is_in_world(bool value) { is_in_world_ = value; }
  bool is_in_world() const { return is_in_world_; }

  Actor(const Actor& other) = delete;
  Actor& operator=(const Actor& other) = delete;
  Actor& operator=(Actor&& other) = delete;

private:
  World& world_;
  uint32_t actor_id_ = 0;
  bool is_in_world_ = false;
};

}