#pragma once

namespace cnc {

struct Float2 {
  Float2(float x, float y);

  Float2& operator+=(const Float2& other);
  Float2& operator-=(const Float2& other);
  Float2& operator*=(int32_t factor);
  Float2& operator/=(int32_t divisor);

  float x = 0.0f;
  float y = 0.0f;
};

bool operator==(const Float2& lhs, const Float2& rhs);
bool operator!=(const Float2& lhs, const Float2& rhs);
Float2 operator+(const Float2& lhs, const Float2& rhs);
Float2 operator-(const Float2& lhs, const Float2& rhs);
Float2 operator*(const Float2& size, int32_t factor);
Float2 operator*(int32_t factor, const Float2& size);
Float2 operator/(const Float2& size, int32_t divisor);

std::ostream& operator<<(std::ostream& stream, const Float2& size);

}
