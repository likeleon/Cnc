#include "cnc/stdafx.h"
#include "cnc/widget_utils.h"
#include "cnc/rectangle.h"
#include "cnc/sprite.h"
#include "cnc/point.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/sprite_renderer.h"
#include "cnc/chrome_provider.h"

namespace cnc {


bool HasFlags(PanelSides a, PanelSides b) {
  using T = std::underlying_type_t<PanelSides>;
  return (static_cast<T>(a) & static_cast<T>(b)) == static_cast<T>(b);
}

PanelSides operator|(PanelSides a, PanelSides b) {
  using T = std::underlying_type_t<PanelSides>;
  return static_cast<PanelSides>(static_cast<T>(a) | static_cast<T>(b));
}

void WidgetUtils::DrawRGBA(const Sprite& s, const Float2& pos) {
  Game::renderer()->rgba_sprite_renderer().DrawSprite(s, pos);
}

void WidgetUtils::DrawPanel(const std::string& collection, const Rectangle& bounds) {
  DrawPanelPartial(collection, bounds, PanelSides::All);
}

void WidgetUtils::DrawPanelPartial(const std::string& collection, const Rectangle& bounds, PanelSides ps) {
  DrawPanelPartial(bounds, ps,
                   ChromeProvider::GetImage(collection, "border-t"),
                   ChromeProvider::GetImage(collection, "border-b"),
                   ChromeProvider::GetImage(collection, "border-l"),
                   ChromeProvider::GetImage(collection, "border-r"),
                   ChromeProvider::GetImage(collection, "corner-tl"),
                   ChromeProvider::GetImage(collection, "corner-tr"),
                   ChromeProvider::GetImage(collection, "corner-bl"),
                   ChromeProvider::GetImage(collection, "corner-br"),
                   ChromeProvider::GetImage(collection, "background"));
}

void WidgetUtils::DrawPanelPartial(const Rectangle& bounds,
                                   PanelSides ps,
                                   const Sprite* border_top,
                                   const Sprite* border_bottom,
                                   const Sprite* border_left,
                                   const Sprite* border_right,
                                   const Sprite* corner_top_left,
                                   const Sprite* corner_top_right,
                                   const Sprite* corner_bottom_left,
                                   const Sprite* corner_bottom_right,
                                   const Sprite* background) {
  int32_t margin_left = (border_left == nullptr) ? 0 : static_cast<int32_t>(border_left->size.x);
  int32_t margin_top = (border_top == nullptr) ? 0 : static_cast<int32_t>(border_top->size.y);
  int32_t margin_right = (border_right == nullptr) ? 0 : static_cast<int32_t>(border_right->size.x);
  int32_t margin_bottom = (border_bottom == nullptr) ? 0 : static_cast<int32_t>(border_bottom->size.y);
  int32_t margin_width = margin_right + margin_left;
  int32_t margin_height = margin_bottom + margin_top;

  if (HasFlags(ps, PanelSides::Center) && background != nullptr) {
    FillRectWithSprite({
      bounds.Left() + margin_left,
      bounds.Top() + margin_top,
      bounds.width - margin_width,
      bounds.height - margin_height
    }, *background);
  }

  if (HasFlags(ps, PanelSides::Left) && border_left != nullptr) {
    FillRectWithSprite({
      bounds.Left(),
      bounds.Top() + margin_top,
      margin_left,
      bounds.height - margin_height
    }, *border_left);
  }

  if (HasFlags(ps, PanelSides::Right) && border_right != nullptr) {
    FillRectWithSprite({
      bounds.Right() - margin_right,
      bounds.Top() + margin_top,
      margin_right,
      bounds.height - margin_height
    }, *border_right);
  }

  if (HasFlags(ps, PanelSides::Top) && border_top != nullptr) {
    FillRectWithSprite({
      bounds.Left() + margin_left,
      bounds.Top(),
      bounds.width - margin_width,
      margin_top
    }, *border_top);
  }

  if (HasFlags(ps, PanelSides::Bottom) && border_bottom != nullptr) {
    FillRectWithSprite({
      bounds.Left() + margin_left,
      bounds.Bottom() - margin_bottom,
      bounds.width - margin_width,
      margin_bottom
    }, *border_bottom);
  }

  if (HasFlags(ps, PanelSides::Left | PanelSides::Top) && corner_top_left != nullptr) {
    DrawRGBA(*corner_top_left, { 
      static_cast<float>(bounds.Left()),
      static_cast<float>(bounds.Top())
    });
  }
  if (HasFlags(ps, PanelSides::Right | PanelSides::Top) && corner_top_right != nullptr) {
    DrawRGBA(*corner_top_right, { 
      static_cast<float>(bounds.Right() - corner_top_right->size.x),
      static_cast<float>(bounds.Top())
    });
  }
  if (HasFlags(ps, PanelSides::Left | PanelSides::Bottom) && corner_bottom_left != nullptr) {
    DrawRGBA(*corner_bottom_left, { 
      static_cast<float>(bounds.Left()),
      static_cast<float>(bounds.Bottom() - corner_bottom_left->size.y)
    });
  }
  if (HasFlags(ps, PanelSides::Right | PanelSides::Bottom) && corner_bottom_right != nullptr) {
    DrawRGBA(*corner_bottom_right, { 
      static_cast<float>(bounds.Right() - corner_bottom_right->size.x),
      static_cast<float>(bounds.Bottom() - corner_bottom_right->size.y)
    });
  }
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