#pragma once

namespace cnc {

struct Rectangle;
struct Sprite;
struct Float2;

class CNC_API WidgetUtils {
public:
  WidgetUtils() = delete;

  static void DrawRGBA(const Sprite& s, const Float2& pos);
  static void FillRectWithSprite(const Rectangle& r, const Sprite& s);
};

}