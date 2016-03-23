#include "cnc/stdafx.h"
#include "cnc/ruleset_cache.h"
#include "cnc/ruleset.h"
#include "cnc/mod_data.h"
#include "cnc/perf_timer.h"
#include "cnc/game.h"
#include "cnc/map.h"
#include "cnc/actor_info.h"

namespace cnc {

static MiniYamlNodes NoMapRules;

RulesetCache::RulesetCache(ModData& mod_data)
  : mod_data_(mod_data) {
}

std::unique_ptr<Ruleset> RulesetCache::Load(Map* map) {
  auto& m = mod_data_.manifest();

  std::map<std::string, ActorInfoPtr> actors;
  {
    PerfTimer p("Actors");
    actors = LoadYamlRules<ActorInfo>(actor_cache_, m.rules(),
                                      map != nullptr ? map->rule_definitions() : NoMapRules,
                                      [](const auto& k, const auto& y) {
      return std::make_shared<ActorInfo>(Game::mod_data()->object_creator(),
                                         StringUtils::ToLower(k.key()), k.value(), y);
    });
  }

  return std::make_unique<Ruleset>(std::move(actors));
}

}