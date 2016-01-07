#pragma once

namespace cnc {

struct Color;

struct HSLColor {
public:
  static bool TryParseRGB(const std::string& value, Color& color);
};

}
