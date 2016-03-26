#pragma once

#include "cnc/type_dictionary.h"

namespace cnc {

class ObjectCreator;
class MiniYaml;

class ActorInfo {
public:
  ActorInfo(ObjectCreator& creator, const std::string& name, const MiniYaml& node,
            const std::map<std::string, MiniYaml>& all_units);

private:
  std::string name_;
  TypeDictionary traits_;
};

}
