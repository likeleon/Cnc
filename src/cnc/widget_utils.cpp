#include "cnc/stdafx.h"
#include "cnc/widget_utils.h"
#include "cnc/rectangle.h"
#include "cnc/sprite.h"
#include "cnc/point.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/sprite_renderer.h"

namespace cnc {

void WidgetUtils::DrawRGBA(const Sprite& /*s*/, const Float2& /*pos*/) {
  //Game::renderer()->rgba_sprite_renderer().DrawSprite(s, pos);
}

void WidgetUtils::FillRectWithSprite(const Rectangle& r, const Sprite& s) {
  auto size_x = static_cast<int32_t>(s.size.x);
  auto size_y = static_cast<int32_t>(s.size.y);
  for (auto x = r.Left(); x < r.Right(); x += size_x) {
    for (auto y = r.Top(); y < r.Bottom(); y += size_y) {
      Point left{ r.Right() - x, r.Bottom() - y };
      if (left.x < size_x || left.y < size_y) {
        Rectangle rr{
          s.bounds.Left(),
          s.bounds.Top(),
          std::min(left.x, size_x),
          std::min(left.y, size_y)
        };
        Sprite ss(s.sheet, rr, s.channel);
        DrawRGBA(ss, Point{ x, y });
      } else {
        DrawRGBA(s, Point{ x, y });
      }
    }
  }
}

}