#pragma once

namespace cnc {

struct Point;
struct Size;

struct CNC_API Rectangle {
  Rectangle();
  Rectangle(const Point& location, const Size& size);
  Rectangle(int32_t x, int32_t y, int32_t width, int32_t height);

  static Rectangle Union(const Rectangle& a, const Rectangle& b);

  static const Rectangle Empty;

  int32_t Left() const;
  int32_t Right() const;
  int32_t Top() const;
  int32_t Bottom() const;
  Size Size() const;

  bool Contains(int32_t x, int32_t y) const;
  bool Contains(const Point& pt) const;
  bool Contains(const Rectangle& rect) const;

  int32_t x = 0;
  int32_t y = 0;
  int32_t width = 0;
  int32_t height = 0;
};

std::ostream& operator<<(std::ostream& os, const Rectangle& rect);

}
