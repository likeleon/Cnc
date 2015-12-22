#pragma once

namespace cnc {

class Sheet;
struct Rectangle;

class GraphicsUtil {
public:
  GraphicsUtil() = delete;

  static void FastCopyIntoSprite(std::string& dest_data,
                                 int32_t dest_stride,
                                 const Rectangle& bounds, 
                                 SDL_Surface* src);
};

}
