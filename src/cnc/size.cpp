#include "cnc/stdafx.h"
#include "cnc/size.h"

namespace cnc {

Size::Size() {
}

Size::Size(int32_t width, int32_t height)
  : width(width), height(height) {
}

inline Size& Size::operator+=(const Size& other) {
  width += other.width;
  height += other.height;
  return *this;
}

inline Size& Size::operator-=(const Size& other) {
  width -= other.width;
  height -= other.height;
  return *this;
}

inline Size& Size::operator*=(int32_t factor) {
  width *= factor;
  height *= factor;
  return *this;
}

inline Size& Size::operator/=(int32_t divisor) {
  width /= divisor;
  height /= divisor;
  return *this;
}

bool operator==(const Size& lhs, const Size& rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

bool operator!=(const Size& lhs, const Size& rhs) {
  return !(lhs == rhs);
}

Size operator+(const Size& lhs, const Size& rhs) {
  return{ lhs.width + rhs.width, lhs.height + rhs.height };
}

Size operator-(const Size& lhs, const Size& rhs) {
  return{ lhs.width - rhs.width, lhs.height - rhs.height };
}

Size operator*(const Size& size, int32_t factor) {
  return{ size.width * factor, size.height * factor };
}

Size operator*(int32_t factor, const Size& size) {
  return size * factor;
}

Size operator/(const Size& size, int32_t divisor) {
  return{ size.width / divisor, size.height / divisor };
}

std::ostream& operator<<(std::ostream& stream, const Size& size) {
  stream << '(' << size.width << ',' << size.height << ')';
  return stream;
}

}