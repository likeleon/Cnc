#include "cnc/stdafx.h"
#include "cnc/float2.h"

namespace cnc {

const Float2 Float2::Zero(0.0f, 0.0f);

Float2::Float2(float x, float y)
  : x(x), y(y) {
}

Float2& Float2::operator+=(const Float2& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Float2& Float2::operator-=(const Float2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Float2& Float2::operator*=(int32_t factor) {
  x *= factor;
  y *= factor;
  return *this;
}

Float2& Float2::operator/=(int32_t divisor) {
  x /= divisor;
  y /= divisor;
  return *this;
}

bool operator==(const Float2& lhs, const Float2& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Float2& lhs, const Float2& rhs) {
  return !(lhs == rhs);
}

Float2 operator+(const Float2& lhs, const Float2& rhs) {
  return{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Float2 operator-(const Float2& lhs, const Float2& rhs) {
  return{ lhs.x - rhs.x, lhs.y - rhs.y };
}

Float2 operator*(const Float2& f2, int32_t factor) {
  return{ f2.x * factor, f2.y * factor };
}

Float2 operator*(int32_t factor, const Float2& f2) {
  return f2 * factor;
}

Float2 operator/(const Float2& f2, int32_t divisor) {
  return{ f2.x / divisor, f2.y / divisor };
}

std::ostream& operator<<(std::ostream& stream, const Float2& f2) {
  stream << f2.x << "," << f2.y;
  return stream;
}

}