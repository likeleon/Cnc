#pragma once

#include "cnc/actor_info.h"
#include "cnc/action.h"
#include "cnc/string_utils.h"

namespace cnc {

class ModData;
class RuleSet;
class Map;

class RulesetCache {
public:
  explicit RulesetCache(ModData& mod_data);

  std::unique_ptr<RuleSet> Load(Map* map = nullptr) const;

  RulesetCache(const RulesetCache& other) = delete;
  RulesetCache& operator=(const RulesetCache& other) = delete;

private:
  template <typename T>
  std::map<std::string> LoadYamlRules(std::map<std::string, std::shared_ptr<T>>& item_cache,
                                      const std::vector<std::string>& files,
                                      const MiniYamlNodes& nodes,
                                      Func<const MiniYamlNode&, std::shared_ptr<T>> f) {
  }

  ModData& mod_data_;
  std::map<std::string, ActorInfoUniquePtr> actor_cache_;
};

}
