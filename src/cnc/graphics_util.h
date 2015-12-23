#pragma once

namespace cnc {

class Sheet;
struct Rectangle;
struct Color;

class GraphicsUtil {
public:
  GraphicsUtil() = delete;

  static void FastCopyIntoSprite(std::string& dest_data,
                                 int32_t dest_stride,
                                 const Rectangle& bounds, 
                                 SDL_Surface* src);
  static Color PremultiplyAlpha(const Color& c);
};

}
