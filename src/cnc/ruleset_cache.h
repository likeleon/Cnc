#pragma once

#include "cnc/action.h"
#include "cnc/string_utils.h"
#include "cnc/mini_yaml.h"
#include "cnc/container_utils.h"

namespace cnc {

class ModData;
class Ruleset;
class Map;

class RulesetCache {
public:
  explicit RulesetCache(ModData& mod_data);

  std::shared_ptr<Ruleset> Load(Map* map = nullptr);

  RulesetCache(const RulesetCache& other) = delete;
  RulesetCache& operator=(const RulesetCache& other) = delete;

private:
  template <typename T>
  std::map<std::string, std::shared_ptr<T>> LoadYamlRules(
    std::map<std::string, std::shared_ptr<T>>& item_cache,
    const std::vector<std::string>& files,
    const MiniYamlNodes& nodes,
    Func<std::shared_ptr<T>, const MiniYamlNode&, const MiniYamlMap&> f) {
    auto input_key = StringUtils::Join(files, "|") + "|" + MiniYaml::WriteToString(nodes);

    auto yy = MiniYaml::FromFiles(files);
    auto accumulator = [](const auto& a, const auto& b) { return MiniYaml::MergePartial(a, b); };
    auto partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);

    Func<std::shared_ptr<T>, const MiniYamlNode&, const MiniYamlMap&> wrap =
      [&item_cache, f, input_key](const auto& wkv, const auto& wyy) {
      auto key = input_key + StringUtils::Join(wkv.value().ToLines(wkv.key()), "|");
      
      auto iter = item_cache.find(key);
      if (iter != item_cache.end()) {
        return iter->second;
      }
      
      auto t = f(wkv, wyy);
      item_cache.emplace(key, t);

      return t;
    };

    MiniYamlMap yyy;
    for (const auto& p : partial) {
      yyy.emplace(p.key(), p.value());
    }
    auto item_set = ToMapWithConflictLog<MiniYamlNode, std::string, std::shared_ptr<T>>(
      partial,
      [](const auto& kv) { return StringUtils::ToLower(kv.key()); },
      [wrap, &yyy](const auto& kv) { return wrap(kv, yyy); },
      "LoadYamlRules", nullptr, nullptr);

    return item_set;
  }

  std::map<std::string, TileSetPtr> LoadTileSets(std::map<std::string, TileSetPtr>& item_cache, 
                                                 const std::vector<std::string>& files);

  ModData& mod_data_;
  std::map<std::string, ActorInfoPtr> actor_cache_;
  std::map<std::string, TileSetPtr> tile_set_cache_;
};

}
