#pragma once

#include "cnc/sprite_loader.h"

namespace cnc {
namespace mods {
namespace common {

class ShpTDLoader : public ISpriteLoader {
public:
  bool TryParseSprite(const std::vector<char>& s, std::vector<ISpriteFramePtr>& frames) override;
};

}
}
}