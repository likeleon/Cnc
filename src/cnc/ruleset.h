#pragma once

namespace cnc {

class Ruleset {
public:
  Ruleset(std::map<std::string, ActorInfoPtr>&& actors,
          std::map<std::string, TileSetPtr>&& tile_sets);

  const std::map<std::string, ActorInfoPtr>& actors() const { return actors_; }
  const std::map<std::string, TileSetPtr>& tile_sets() const { return tile_sets_; }
  
private:
  std::map<std::string, ActorInfoPtr> actors_;
  std::map<std::string, TileSetPtr> tile_sets_;
};

}
