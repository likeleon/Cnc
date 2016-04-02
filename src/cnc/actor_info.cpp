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

  // TODO: Remove null check
  if (!creator.TypeRegistered(trait_name + "Info")) {
    return nullptr;
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
          // TODO: Remove null check
          auto trait = LoadTraitInfo(creator, StringUtils::Split(t.key(), '@')[0], t.value());
          if (trait != nullptr) {
            traits_.Add(trait);
          }
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

struct TraitTypeInfo {
  ITraitInfoPtr trait;
  std::vector<ITraitInfoPtr> dependencies;
};

static std::vector<ITraitInfoPtr> PrerequisitesOf(const ITraitInfo& info) {
  auto types = info.Types();
  if (std::find(types.begin(), types.end(), typeid(Requires)) == types.end()) {
    return{};
  }
  return reinterpret_cast<const Requires&>(info).RequiredTypes();
}

static bool IsAssignable(const ITypeExposable& t, const ITypeExposable& from) {
  const auto& this_type = typeid(t);
  if (this_type == typeid(from)) {
    return true;
  }
  
  const auto& base_types = from.Types();
  if (std::find(base_types.begin(), base_types.end(), this_type) != base_types.end()) {
    return true;
  }

  return false;
}

const std::vector<ITraitInfoPtr>& ActorInfo::TraitsInConstructionOrder() {
  if (construct_order_cache_) {
    return construct_order_cache_.value();
  }

  std::vector<TraitTypeInfo> source, resolved, unresolved;
  for (const auto& t : traits_.WithInterface<ITraitInfo>()) {
    TraitTypeInfo type_info{ t, PrerequisitesOf(*t) };
    source.emplace_back(type_info);
    if (type_info.dependencies.empty()) {
      resolved.emplace_back(type_info);
    } else {
      unresolved.emplace_back(type_info);
    }
  }

  auto test_resolve = [](const ITraitInfo& a, const ITraitInfo& b) { return IsAssignable(a, b); };
  for (;;) {
    std::vector<TraitTypeInfo> more;
    auto test_resolve_any = [&](const ITraitInfoPtr& t) {
      return std::any_of(resolved.begin(), resolved.end(), [&](const auto& r) { return test_resolve(*t, *r.trait); });
    };
    auto test_resolve_type = [&](const TraitTypeInfo& t) {
      return std::all_of(t.dependencies.begin(), t.dependencies.end(), [&](const auto& i) { return test_resolve_any(i); });
    };
    for (auto iter = unresolved.begin(); iter != unresolved.end();) {
      if (!test_resolve_type(*iter)) {
        ++iter;
        continue;
      }
      more.emplace_back(*iter);
      iter = unresolved.erase(iter);
    }

    if (more.empty()) {
      break;
    }
  }

  if (!unresolved.empty()) {
    std::ostringstream oss;
    oss << "ActorInfo(" << name_ << ") failed to initialize because of the following:\r\n";
    std::set<ITraitInfoPtr> missing;
    for (const auto& u : unresolved) {
      for (const auto& d : u.dependencies) {
        if (std::any_of(source.begin(), source.end(), [&](const auto& i) { return test_resolve(*d, *i.trait); })) {
          continue;
        }
        missing.emplace(d);
      }
    }

    oss << "Missing:\r\n";
    for (const auto& m : missing) {
      oss << typeid(*m).name() << "\r\n";
    }

    oss << "Unresolved:\r\n";
    for (const auto& u : unresolved) {
      std::vector<ITraitInfoPtr> deps;
      std::copy_if(u.dependencies.begin(), u.dependencies.end(), std::back_inserter(deps), [&](const auto& d) {
        return !std::any_of(resolved.begin(), resolved.end(), [&](const auto& r) { return r.trait == d; });
      });
      std::vector<std::string> deps_names;
      std::transform(deps.begin(), deps.end(), std::back_inserter(deps_names), [](const auto& i) { return typeid(*i).name(); });
      oss << typeid(*u.trait).name() << ": {" << StringUtils::Join(deps_names, ", ") << " }\r\n";
    }

    throw Error(MSG(oss.str()));
  }

  construct_order_cache_ = std::vector<ITraitInfoPtr>{};
  std::transform(resolved.begin(), resolved.end(), std::back_inserter(construct_order_cache_.value()),
                 [](const auto& i) { return i.trait; });
  return construct_order_cache_.value();
}

}