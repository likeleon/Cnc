#include "cnc/stdafx.h"
#include "cnc/hsl_color.h"
#include "cnc/string_utils.h"
#include "cnc/color.h"

namespace cnc {

bool HSLColor::TryParseRGB(const std::string& v, Color& color) {
  auto value = StringUtils::Trim(v);
  if (value.length() != 6 && value.length() != 8) {
    return false;
  }

  int32_t red = std::stoi(value.substr(0, 2), 0, 16);
  int32_t green = std::stoi(value.substr(2, 2), 0, 16);
  int32_t blue = std::stoi(value.substr(4, 2), 0, 16);
  
  int32_t alpha = 255;
  if (value.length() == 8) {
    alpha = std::stoi(value.substr(6, 2), 0, 16);
  }
  
  color = { red, green, blue, alpha };
  return true;
}

}
