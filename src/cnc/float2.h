#pragma once

namespace cnc {

struct Point;
struct Size;

struct CNC_API Float2 {
  Float2(float x, float y);
  Float2(int32_t x, int32_t y);
  Float2(const Point& p);
  Float2(const Size& sz);

  Float2& operator+=(const Float2& other);
  Float2& operator-=(const Float2& other);
  Float2& operator*=(float factor);
  Float2& operator/=(float divisor);

  Point ToPoint() const;
  Size ToSize() const;

  static const Float2 Zero;

  float x = 0.0f;
  float y = 0.0f;
};

CNC_API bool operator==(const Float2& lhs, const Float2& rhs);
CNC_API bool operator!=(const Float2& lhs, const Float2& rhs);
CNC_API Float2 operator+(const Float2& lhs, const Float2& rhs);
CNC_API Float2 operator-(const Float2& lhs, const Float2& rhs);
CNC_API Float2 operator*(const Float2& f2, float factor);
CNC_API Float2 operator*(float factor, const Float2& f2);
CNC_API Float2 operator*(const Float2& f2, const Float2& factor);
CNC_API Float2 operator*(const Float2& factor, const Float2& f2);
CNC_API Float2 operator/(const Float2& f2, float divisor);
CNC_API Float2 operator/(const Float2& f2, const Float2& divisor);

CNC_API std::ostream& operator<<(std::ostream& stream, const Float2& size);

}
