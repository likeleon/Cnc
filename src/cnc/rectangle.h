#pragma once

namespace cnc {

struct Point;
struct Size;

struct Rectangle {
  Rectangle(const Point& location, const Size& size);
  Rectangle(int32_t x, int32_t y, int32_t width, int32_t height);

  static const Rectangle Empty;

  int32_t Left() const;
  int32_t Right() const;
  int32_t Top() const;
  int32_t Bottom() const;
  Size Size() const;

  int32_t x = 0;
  int32_t y = 0;
  int32_t width = 0;
  int32_t height = 0;
};

std::ostream& operator<<(std::ostream& os, const Rectangle& rect);

}
