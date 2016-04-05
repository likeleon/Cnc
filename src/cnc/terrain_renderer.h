#pragma once

namespace cnc {

class World;
class WorldRenderer;
class TerrainSpriteLayer;
class Viewport;

class TerrainRenderer {
public:
  TerrainRenderer(World& world, WorldRenderer& wr);
  ~TerrainRenderer();

  void Draw(WorldRenderer& wr, const Viewport& viewport);

private:
  std::unique_ptr<TerrainSpriteLayer> terrain_;
};

}
