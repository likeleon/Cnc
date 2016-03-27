#include "cnc/stdafx.h"
#include "cnc/actor_info.h"
#include "cnc/string_utils.h"
#include "cnc/mini_yaml.h"
#include "cnc/field_loader.h"
#include "cnc/traits_interfaces.h"
#include "cnc/object_creator.h"

namespace cnc {

static ITraitInfoPtr LoadTraitInfo(ObjectCreator& creator, const std::string& trait_name, const MiniYaml& my) {
  if (!my.value().empty()) {
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

static MiniYamlMap GetParents(const MiniYaml& node, const MiniYamlMap& all_units) {
  MiniYamlMap ret;
  for (const auto& n : node.nodes()) {
    if (n.key() != "Inherits" && !StringUtils::StartsWith(n.key(), "Inherits@")) {
      continue;
    }

    auto iter = all_units.find(n.value().value());
    if (iter == all_units.end()) {
      throw YamlException(MSG("Bogus inheritance -- parent type " + n.value().value() + " does not exist"));
    }

    ret.emplace(n.value().value(), iter->second);
  }
  return ret;
}

static MiniYaml MergeWithParents(MiniYaml node, const MiniYamlMap& all_units, std::set<std::string>& all_parents) {
  auto parents = GetParents(node, all_units);

  for (const auto& kv : parents) {
    if (!all_parents.insert(kv.first).second) {
      throw YamlException(MSG("Bogus inheritance -- duplicate inheritance of " + kv.first + "."));
    }

    node = MiniYaml::MergePartial(node, MergeWithParents(kv.second, all_units, all_parents));
  }

  return node;
}

ActorInfo::ActorInfo(ObjectCreator& creator, const std::string& name, const MiniYaml& node, const MiniYamlMap& all_units)
  : name_(name) {
  try {
    std::set<std::string> all_parents;
    auto abstract_actor_type = StringUtils::StartsWith(name, "^");

    all_parents.insert(name); // guard against circular inheritance

    auto partial = MergeWithParents(node, all_units, all_parents);
    for (const auto& t : MiniYaml::ApplyRemovals(partial.nodes())) {
      if (t.key() != "Inherits" && !StringUtils::StartsWith(t.key(), "Inherits@")) {
        try {
          traits_.Add(LoadTraitInfo(creator, StringUtils::Split(t.key(), '@')[0], t.value()));
        } catch (const FieldLoader::MissingFieldsException& e) {
            if (!abstract_actor_type) {
              throw YamlException(MSG(e.message().text));
            }
          }
      }
    }
  } catch (const YamlException& e) {
    throw YamlException(MSG("Actor type " + name_ + ": " + e.message().text));
  }
}

}