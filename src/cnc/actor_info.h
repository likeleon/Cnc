#pragma once

#include "cnc/type_dictionary.h"
#include "cnc/mini_yaml.h"

namespace cnc {

class ObjectCreator;
class MiniYaml;

class ActorInfo {
public:
  ActorInfo(ObjectCreator& creator, const std::string& name, const MiniYaml& node, const MiniYamlMap& all_units);

  const std::vector<ITraitInfoPtr>& TraitsInConstructionOrder();

private:
  std::string name_;
  TypeDictionary traits_;
  optional<std::vector<ITraitInfoPtr>> construct_order_cache_;
};

}
