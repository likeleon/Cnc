#pragma once

namespace cnc {

struct Size {
  Size();
  Size(int32_t width, int32_t height);

  Size& operator+=(const Size& other);
  Size& operator-=(const Size& other);
  Size& operator*=(int32_t factor);
  Size& operator/=(int32_t divisor);

  int32_t width = 0;
  int32_t height = 0;
};

bool operator==(const Size& lhs, const Size& rhs);
bool operator!=(const Size& lhs, const Size& rhs);
Size operator+(const Size& lhs, const Size& rhs);
Size operator-(const Size& lhs, const Size& rhs);
Size operator*(const Size& size, int32_t factor);
Size operator*(int32_t factor, const Size& size);
Size operator/(const Size& size, int32_t divisor);

std::ostream& operator<<(std::ostream& os, const Size& size);

}
