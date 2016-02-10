#include "cnc/stdafx.h"
#include "cnc/color.h"

namespace cnc {

const Color Color::Transparent(0, 0, 0, 0);
const Color Color::Black(0, 0, 0);
const Color Color::Gray(128, 128, 128);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);

Color::Color(int32_t argb)
  : r((argb >> 16) & 0xFF),
  g((argb >> 8) & 0xFF),
  b((argb >> 0) & 0xFF),
  a((argb >> 24) & 0xFF) {
}

Color::Color(int32_t a, const Color& base_color)
  : Color(base_color.r, base_color.g, base_color.b, a) {
}

Color::Color(int32_t r, int32_t g, int32_t b, int32_t a)
  : r(static_cast<uint8_t>(r)),
  g(static_cast<uint8_t>(g)),
  b(static_cast<uint8_t>(b)),
  a(static_cast<uint8_t>(a)) {
}

int32_t Color::ToArgb() const {
  return (a << 24) + (r << 16) + (g << 8) + b;
}

bool operator==(const Color& lhs, const Color& rhs) {
  return lhs.r == rhs.r &&
    lhs.g == rhs.g &&
    lhs.b == rhs.b &&
    lhs.a == rhs.a;
}

bool operator!=(const Color &lhs, const Color& rhs) {
  return lhs.r != rhs.r ||
    lhs.g != rhs.g ||
    lhs.b != rhs.b ||
    lhs.a != rhs.a;
}

}