#include "cnc/stdafx.h"
#include "cnc/ruleset.h"

namespace cnc {

Ruleset::Ruleset(std::map<std::string, ActorInfoPtr>&& actors,
                 std::map<std::string, TileSetPtr>&& tile_sets)
  : actors_(std::move(actors)), tile_sets_(std::move(tile_sets)) {
}

}