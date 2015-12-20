#include "cnc/stdafx.h"
#include "cnc/float2.h"
#include "cnc/size.h"

namespace cnc {

const Float2 Float2::Zero(0.0f, 0.0f);

Float2::Float2(float x, float y)
  : x(x), y(y) {
}

Float2::Float2(const Size& sz)
  : Float2(static_cast<float>(sz.width), static_cast<float>(sz.height)) {
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

Float2& Float2::operator*=(float factor) {
  x *= factor;
  y *= factor;
  return *this;
}

Float2& Float2::operator/=(float divisor) {
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

Float2 operator*(const Float2& f2, float factor) {
  return{ f2.x * factor, f2.y * factor };
}

Float2 operator*(float factor, const Float2& f2) {
  return f2 * factor;
}

Float2 operator*(const Float2& a, const Float2& b) {
  return{ a.x * b.x, a.y * b.y };
}

Float2 operator/(const Float2& f2, float divisor) {
  return{ f2.x / divisor, f2.y / divisor };
}

Float2 operator/(const Float2& f2, const Float2& divisor) {
  return{ f2.x / divisor.x, f2.y / divisor.y };
}

std::ostream& operator<<(std::ostream& stream, const Float2& f2) {
  stream << f2.x << "," << f2.y;
  return stream;
}

}