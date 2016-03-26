#include "cnc/stdafx.h"
#include "cnc/actor_info.h"
#include "cnc/string_utils.h"
#include "cnc/mini_yaml.h"
#include "cnc/field_loader.h"
#include "cnc/traits_interfaces.h"
#include "cnc/object_creator.h"

namespace cnc {

static ITraitInfoPtr LoadTraitInfo(ObjectCreator& creator, const std::string& trait_name, const MiniYaml& my) {
  if (my.value().empty()) {
    std::ostringstream oss;
    oss << "Junk value '" << my.value() << "' on trait node " << trait_name;
    throw YamlException(MSG(oss.str()));
  }

  auto info = creator.CreateObject<ITraitInfo>(trait_name + "Info");
  try {
    FieldLoader::Load(*info, my);
  } catch (const FieldLoader::MissingFieldsException& e) {
    auto missing = e.missing();
    auto header = "Trait name " + trait_name + ": " + (missing.size() > 1 ? "Required properties missing" : "Required property missing");
    throw FieldLoader::MissingFieldsException(MSG(header), std::move(missing));
  }

  return info;
}

ActorInfo::ActorInfo(ObjectCreator& creator, const std::string& name, const MiniYaml& node,
                     const std::map<std::string, MiniYaml>& /*all_units*/)
  : name_(name) {
  try {
    auto abstract_actor_type = StringUtils::StartsWith(name, "^");
    for (const auto& t : node.nodes()) {
      try {
        traits_.Add(LoadTraitInfo(creator, StringUtils::Split(t.key(), '@')[0], t.value()));
      } catch (const FieldLoader::MissingFieldsException& e) {
        if (!abstract_actor_type) {
          throw YamlException(MSG(e.message().text));
        }
      }
    }
  } catch (const YamlException& e) {
    throw YamlException(MSG("Actor type " + name_ + ": " + e.message().text));
  }
}

}