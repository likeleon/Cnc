#include "cnc/stdafx.h"
#include "cnc/sprite_loader.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"

namespace cnc {

SpriteCache::SpriteCache(const std::vector<SpriteLoaderPtr>& loaders, std::unique_ptr<SheetBuilder> sheet_builder)
  : loaders_(loaders), sheet_builder_(std::move(sheet_builder)) {
}

const std::vector<Sprite>& SpriteCache::operator[](const std::string& filename) {
  auto iter = sprites_.find(filename);
  if (iter == sprites_.end()) {
    auto sprites = SpriteLoader::GetSprites(filename, loaders_, *sheet_builder_);
    sprites_.emplace(filename, std::move(sprites));
  }
  return iter->second;
}

std::vector<Sprite> SpriteLoader::GetSprites(const std::string& filename, 
                                             const std::vector<SpriteLoaderPtr>& loaders, 
                                             SheetBuilder& sheet_builder) {
  std::vector<Sprite> sprites;
  auto frames = GetFrames(filename, loaders);
  std::transform(frames.begin(), frames.end(), std::back_inserter(sprites), 
                 [&sheet_builder](const auto& frame) { return sheet_builder.Add(*frame); });
  return sprites;
}

std::vector<ISpriteFrameUniquePtr> SpriteLoader::GetFrames(const std::string& filename,
                                                           const std::vector<SpriteLoaderPtr>& loaders) {
  auto stream = Game::mod_data()->mod_files().Open(filename);
  for (const auto& loader : loaders) {
    std::vector<ISpriteFrameUniquePtr> frames;
    if (loader->TryParseSprite(stream, frames)) {
      return frames;
    }
  }

  throw Error(MSG(filename + " is not a valid sprite file!"));
}

}
