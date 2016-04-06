#include "cnc/stdafx.h"
#include "cnc/ruleset_cache.h"
#include "cnc/ruleset.h"
#include "cnc/mod_data.h"
#include "cnc/perf_timer.h"
#include "cnc/game.h"
#include "cnc/map.h"
#include "cnc/actor_info.h"
#include "cnc/tile_set.h"

namespace cnc {

static MiniYamlNodes NoMapRules;

RulesetCache::RulesetCache(ModData& mod_data)
  : mod_data_(mod_data) {
}

std::shared_ptr<Ruleset> RulesetCache::Load(Map* map) {
  auto& m = mod_data_.manifest();

  std::map<std::string, ActorInfoPtr> actors;
  std::map<std::string, TileSetPtr> tile_sets;
  {
    PerfTimer p("Actors");
    actors = LoadYamlRules<ActorInfo>(actor_cache_, m.rules(),
                                      map != nullptr ? map->rule_definitions() : NoMapRules,
                                      [](const auto& k, const auto& y) {
      return std::make_shared<ActorInfo>(Game::mod_data()->object_creator(),
                                         StringUtils::ToLower(k.key()), k.value(), y);
    });
  }
  {
    PerfTimer p("TileSets");
    tile_sets = LoadTileSets(tile_set_cache_, m.tile_sets());
  }

  return std::make_shared<Ruleset>(std::move(actors), std::move(tile_sets));
}

std::map<std::string, TileSetPtr> RulesetCache::LoadTileSets(std::map<std::string, TileSetPtr>& item_cache, 
                                                             const std::vector<std::string>& files) {
  std::map<std::string, TileSetPtr> items;

  for (const auto& file : files) {
    TileSetPtr t;
    if (!TryGetValue(item_cache, file, t)) {
      t = std::make_shared<TileSet>(file);
      item_cache.emplace(file, t);
    }
    items.emplace(t->id(), t);
  }
  return items;
}

}