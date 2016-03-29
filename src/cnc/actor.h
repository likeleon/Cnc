#pragma once

namespace cnc {

class World;
class TypeDictionary;

class Actor {
public:
  Actor(World& world, const std::string& name, const TypeDictionary& init_dict);

  Actor(const Actor& other) = delete;
  Actor& operator=(const Actor& other) = delete;
  Actor& operator=(Actor&& other) = delete;

private:
  World& world_;
};

}