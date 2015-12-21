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

}