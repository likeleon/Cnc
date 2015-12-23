#pragma once

namespace cnc {

struct Rectangle;
struct Sprite;

class CNC_API WidgetUtils {
public:
  WidgetUtils() = delete;

  static void FillRectWithSprite(const Rectangle& r, const Sprite& s);
};

}