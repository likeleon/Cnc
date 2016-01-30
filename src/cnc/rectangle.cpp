#include "cnc/stdafx.h"
#include "cnc/rectangle.h"
#include "cnc/point.h"
#include "cnc/size.h"

namespace cnc {

const Rectangle Rectangle::Empty(0, 0, 0, 0);

Rectangle::Rectangle() {
}

Rectangle::Rectangle(const Point& location, const cnc::Size& size)
  : x(location.x), y(location.y), width(size.width), height(size.height) {
}

Rectangle::Rectangle(int32_t x, int32_t y, int32_t width, int32_t height)
  : x(x), y(y), width(width), height(height) {
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
  os << rect.x << ',' << rect.y << ',' << rect.width << ',' << rect.height;
  return os;
}

Rectangle Rectangle::Union(const Rectangle& a, const Rectangle& b) {
  int32_t x1 = std::min(a.x, b.x);
  int32_t x2 = std::max(a.x + a.width, b.x + b.width);
  int32_t y1 = std::min(a.y, b.y);
  int32_t y2 = std::max(a.y + a.height, b.y + b.height);
  return{ x1, y1, x2 - x1, y2 - y1 };
}

int32_t Rectangle::Left() const {
  return x;
}

int32_t Rectangle::Right() const {
  return x + width;
}

int32_t Rectangle::Top() const {
  return y;
}

int32_t Rectangle::Bottom() const {
  return y + height;
}

Size Rectangle::Size() const {
  return{ width, height };
}

bool Rectangle::Contains(int32_t _x, int32_t _y) const {
  return x <= _x && _x < x + width
      && y <= _y && _y < y + height;
}

bool Rectangle::Contains(const Point& pt) const {
  return Contains(pt.x, pt.y);
}

bool Rectangle::Contains(const Rectangle& rect) const {
  return this->x <= rect.x
      && rect.x + rect.width <= this->x + this->width
      && this->y <= rect.y
      && rect.y + rect.height <= this->y + this->height;
}

}