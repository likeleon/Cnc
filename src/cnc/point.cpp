#include "cnc/stdafx.h"
#include "cnc/point.h"
#include "cnc/size.h"
#include "cnc/math_utils.h"

namespace cnc {

const Point Point::Zero(0, 0);

Point::Point(int32_t x, int32_t y)
  : x(x), y(y) {
}

Point::operator Size() const {
  return{ x, y };
}

int32_t Point::LengthSquared() const {
  return x * x + y * y;
}

int32_t Point::Length() const {
  return ISqrt(LengthSquared());
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

Point operator+(const Point& lhs, const Point& rhs) {
  return{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Point operator-(const Point& lhs, const Point& rhs) {
  return{ lhs.x - rhs.x, lhs.y - rhs.y };
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