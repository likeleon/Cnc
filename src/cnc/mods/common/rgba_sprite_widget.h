#pragma once

#include "cnc/widget.h"

namespace cnc {

struct Sprite;

namespace mods {
namespace common {

class RGBASpriteWidget : public Widget {
public:
  RGBASpriteWidget();
  RGBASpriteWidget(const RGBASpriteWidget& other);

  WidgetPtr Clone() const override;
  void Draw() override;

  Func<const Sprite*> get_sprite_;
};

}
}
}
