#pragma once

namespace cnc {

struct Color {
  Color(int32_t argb);
  Color(int32_t a, const Color& base_color);
  Color(int32_t r, int32_t g, int32_t b, int32_t a = 255);

  int32_t ToArgb() const;

  static const Color Transparent;
  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Yellow;
  static const Color Magenta;
  static const Color Cyan;

  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

bool operator==(const Color& lhs, const Color& rhs);
bool operator!=(const Color& lhs, const Color& rhs);

}

namespace std {

template<>
struct hash<cnc::Color> {
  size_t operator()(const cnc::Color& c) const {
    return std::hash<size_t>()(c.ToArgb());
  }
};

}