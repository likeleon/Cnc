#pragma once

namespace cnc {

struct Rectangle;
struct Sprite;
struct Float2;

enum class PanelSides {
  Left = 1,
  Top = 2,
  Right = 4,
  Bottom = 8,
  Center = 16,

  Edges = Left | Top | Right | Bottom,
  All = Edges | Center
};

PanelSides operator|(PanelSides a, PanelSides b);
bool HasFlags(PanelSides a, PanelSides b);

class CNC_API WidgetUtils {
public:
  WidgetUtils() = delete;

  static void DrawRGBA(const Sprite& s, const Float2& pos);
  static void DrawPanel(const std::string& collection, const Rectangle& bounds);
  static void DrawPanelPartial(const std::string& collection, const Rectangle& bounds, PanelSides ps);
  static void DrawPanelPartial(const Rectangle& bounds,
                               PanelSides ps,
                               const Sprite* border_top,
                               const Sprite* border_bottom,
                               const Sprite* border_left,
                               const Sprite* border_right,
                               const Sprite* corner_top_left,
                               const Sprite* corner_top_right,
                               const Sprite* corner_bottom_left,
                               const Sprite* corner_bottom_right,
                               const Sprite* background);

  static void FillRectWithSprite(const Rectangle& r, const Sprite& s);
};

}