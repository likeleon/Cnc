#include "cnc/stdafx.h"
#include "cnc/point.h"
#include "cnc/size.h"

namespace cnc {

Point::Point(int32_t x, int32_t y)
  : x(x), y(y) {
}

Point& Point::operator+=(const Size& sz) {
  x += sz.width;
  y += sz.height;
  return *this;
}

Point& Point::operator-=(const Size& sz) {
  x -= sz.width;
  y -= sz.height;
  return *this;
}

bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Point& lhs, const Point& rhs) {
  return !(lhs == rhs);
}

Point operator+(const Point& pt, const Size& sz) {
  return{ pt.x + sz.width, pt.y + sz.height };
}

Point operator-(const Point& pt, const Size& sz) {
  return{ pt.x - sz.width, pt.y - sz.height };
}

std::ostream& operator<<(std::ostream& os, const Point& pt) {
  os << pt.x << "," << pt.y;
  return os;
}

}