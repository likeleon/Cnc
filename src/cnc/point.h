#pragma once

namespace cnc {

struct Size;

struct CNC_API Point {
  Point(int32_t x, int32_t y);

  Point& operator+=(const Size& sz);
  Point& operator-=(const Size& sz);

  static const Point Zero;

  int32_t x = 0;
  int32_t y = 0;
};

bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);
Point operator+(const Point& pt, const Size& sz);
Point operator-(const Point& pt, const Size& sz);

std::ostream& operator<<(std::ostream& os, const Point& pt);

}
