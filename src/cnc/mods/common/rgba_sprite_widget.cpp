#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/rgba_sprite_widget.h"
#include "cnc/game.h"
#include "cnc/renderer.h"

namespace cnc {
namespace mods {
namespace common {

RGBASpriteWidget::RGBASpriteWidget()
  : get_sprite_([]() { return nullptr; }) {
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
