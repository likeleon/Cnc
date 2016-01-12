#pragma once

#include "cnc/sheet_builder.h"
#include "cnc/cache.h"

namespace cnc {

struct Size;
struct Float2;
struct Sprite;

class ISpriteFrame {
public:
  virtual ~ISpriteFrame() {}

  virtual const Size& size() const = 0;
  virtual const Size& frame_size() const = 0;
  virtual const Float2& offset() const = 0;
  virtual const std::vector<char>& data() const = 0;
  virtual bool disable_export_padding() const = 0;
};

using ISpriteFrameUniquePtr = std::unique_ptr<ISpriteFrame>;

class ISpriteLoader {
public:
  virtual ~ISpriteLoader() {}
  virtual bool TryParseSprite(const std::vector<char>& s, std::vector<ISpriteFrameUniquePtr>& frames) = 0;
};

using SpriteLoaderPtr = std::shared_ptr<ISpriteLoader>;

class SpriteCache {
public:
  SpriteCache(const std::vector<SpriteLoaderPtr>& loaders, std::unique_ptr<SheetBuilder> sheet_builder);
  const std::vector<Sprite>& operator[](const std::string& filename);

private:
  const std::vector<SpriteLoaderPtr>& loaders_;
  std::unique_ptr<SheetBuilder> sheet_builder_;
  std::map<std::string, std::vector<Sprite>> sprites_;
};

class SpriteLoader {
public:
  SpriteLoader() = delete;

  static std::vector<Sprite> GetSprites(const std::string& filename, 
                                        const std::vector<SpriteLoaderPtr>& loaders,
                                        SheetBuilder& sheet_builder);

  static std::vector<ISpriteFrameUniquePtr> GetFrames(const std::string& filename,
                                                      const std::vector<SpriteLoaderPtr>& loaders);
};

}
