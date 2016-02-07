#pragma once

#include "cnc/widget.h"

namespace cnc {

struct Sprite;

namespace mods {
namespace common {

class RGBASpriteWidget : public Widget {
public:
  RGBASpriteWidget();
  void Draw() override;

  Func<const Sprite*> get_sprite_;
};

}
}
}
