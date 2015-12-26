#pragma once

namespace cnc {

class Sheet;
struct Sprite;
struct Rectangle;
struct Color;
struct Vertex;
struct Float2;

class GraphicsUtil {
public:
  GraphicsUtil() = delete;

  static void FastCreateQuad(std::vector<Vertex>& vertices,
                             const Float2& o,
                             const Sprite& r,
                             float palette_texture_index,
                             int32_t nv,
                             const Float2& size);
  static void FastCreateQuad(std::vector<Vertex>& vertices,
                             const Float2& a,
                             const Float2& b,
                             const Float2& c,
                             const Float2& d,
                             const Sprite& r,
                             float palette_texture_index,
                             int32_t nv);
  
  static void FastCopyIntoSprite(std::vector<char>& dest_data,
                                 int32_t dest_stride,
                                 const Rectangle& bounds, 
                                 SDL_Surface* src);
  
  static Color PremultiplyAlpha(const Color& c);
};

}
