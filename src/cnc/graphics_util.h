#pragma once

namespace cnc {

class Sheet;
struct Sprite;
struct Rectangle;
struct Color;
struct Vertex;
struct Float2;
class Bitmap;

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

  static void FastCopyIntoChannel(Sprite& dest, const std::vector<char>& src);
  static void FastCopyIntoChannel(Sprite& dest, int32_t channel_offset, const std::vector<char>& src);
  
  static void FastCopyIntoSprite(std::vector<char>& dest_data,
                                 int32_t dest_stride,
                                 Bitmap* src);
  
  static Color PremultiplyAlpha(const Color& c);
};

}
