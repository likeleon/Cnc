#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/rgba_sprite_widget.h"
#include "cnc/game.h"
#include "cnc/renderer.h"

namespace cnc {
namespace mods {
namespace common {

RGBASpriteWidget::RGBASpriteWidget()
  : get_sprite_([] { return nullptr; }) {
}

RGBASpriteWidget::RGBASpriteWidget(const RGBASpriteWidget& other)
  : Widget(other) {
  get_sprite_ = other.get_sprite_;
}

WidgetPtr RGBASpriteWidget::Clone() const {
  return WidgetPtr(new RGBASpriteWidget(*this));
}

void RGBASpriteWidget::Draw() {
  const auto* sprite = get_sprite_();
  if (sprite != nullptr) {
    Game::renderer()->rgba_sprite_renderer().DrawSprite(*sprite, RenderOrigin());
  }
}

}
}
}
