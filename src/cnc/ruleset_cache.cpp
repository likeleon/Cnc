#include "cnc/stdafx.h"
#include "cnc/ruleset_cache.h"
#include "cnc/ruleset.h"
#include "cnc/mod_data.h"
#include "cnc/perf_timer.h"
#include "cnc/game.h"

namespace cnc {

RulesetCache::RulesetCache(ModData& mod_data) 
  : mod_data_(mod_data) {
}

std::unique_ptr<RuleSet> RulesetCache::Load(Map* map) const {
  auto& m = mod_data_.manifest();

  std::map<std::string, ActorInfo> actors;
  {
    PerfTimer p("Actors");
    actors = LoadYamlRules(actor_cache_, m.rules(), 
                           map != nullptr ? map->rule_definitions() : NoMapRules,
                           [](const auto& k) { 
      return std::make_unique<ActorInfo>(Game::mod_data()->object_creator, );
  }
}

}