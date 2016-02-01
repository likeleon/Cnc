#pragma once

namespace cnc {

struct Size;

struct CNC_API Point {
  Point(int32_t x, int32_t y);

  explicit operator Size() const;

  int32_t LengthSquared() const;
  int32_t Length() const;
  
  Point& operator+=(const Size& sz);
  Point& operator-=(const Size& sz);

  static const Point Zero;

  int32_t x = 0;
  int32_t y = 0;
};

CNC_API Point operator+(const Point& lhs, const Point& rhs);
CNC_API Point operator-(const Point& lhs, const Point& rhs);

CNC_API bool operator==(const Point& lhs, const Point& rhs);
CNC_API bool operator!=(const Point& lhs, const Point& rhs);
CNC_API Point operator+(const Point& pt, const Size& sz);
CNC_API Point operator-(const Point& pt, const Size& sz);

CNC_API std::ostream& operator<<(std::ostream& os, const Point& pt);

}
