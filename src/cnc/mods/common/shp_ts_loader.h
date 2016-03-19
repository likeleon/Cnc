#pragma once

#include "cnc/sprite_loader.h"

namespace cnc {
namespace mods {
namespace common {

class ShpTSLoader : public ISpriteLoader {
public:
  bool TryParseSprite(StreamPtr s, std::vector<ISpriteFramePtr>& frames) override;
};

}
}
}
