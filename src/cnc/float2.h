#pragma once

namespace cnc {

struct Size;

struct Float2 {
  Float2(float x, float y);
  explicit Float2(const Size& sz);

  Float2& operator+=(const Float2& other);
  Float2& operator-=(const Float2& other);
  Float2& operator*=(float factor);
  Float2& operator/=(float divisor);

  static const Float2 Zero;

  float x = 0.0f;
  float y = 0.0f;
};

bool operator==(const Float2& lhs, const Float2& rhs);
bool operator!=(const Float2& lhs, const Float2& rhs);
Float2 operator+(const Float2& lhs, const Float2& rhs);
Float2 operator-(const Float2& lhs, const Float2& rhs);
Float2 operator*(const Float2& f2, float factor);
Float2 operator*(float factor, const Float2& f2);
Float2 operator*(const Float2& f2, const Float2& factor);
Float2 operator*(const Float2& factor, const Float2& f2);
Float2 operator/(const Float2& f2, float divisor);
Float2 operator/(const Float2& f2, const Float2& divisor);

std::ostream& operator<<(std::ostream& stream, const Float2& size);

}
