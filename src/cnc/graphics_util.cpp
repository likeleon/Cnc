#include "cnc/stdafx.h"
#include "cnc/graphics_util.h"
#include "cnc/surface_ptr.h"
#include "cnc/rectangle.h"
#include "cnc/error.h"
#include "cnc/color.h"
#include "cnc/float2.h"
#include "cnc/sprite.h"
#include "cnc/sheet.h"
#include "cnc/vertex.h"

namespace cnc {

void GraphicsUtil::FastCreateQuad(std::vector<Vertex>& vertices,
                           const Float2& o,
                           const Sprite& r,
                           float palette_texture_index,
                           int32_t nv,
                           const Float2& size) {
  Float2 b(o.x + size.x, o.y);
  Float2 c(o.x + size.x, o.y + size.y);
  Float2 d(o.x, o.y + size.y);
  FastCreateQuad(vertices, o, b, c, d, r, palette_texture_index, nv);
}

void GraphicsUtil::FastCreateQuad(std::vector<Vertex>& vertices,
                           const Float2& a,
                           const Float2& b,
                           const Float2& c,
                           const Float2& d,
                           const Sprite& r,
                           float palette_texture_index,
                           int32_t nv) {
  static const std::array<float, 4> ChannelSelect = { 0.2f, 0.4f, 0.6f, 0.8f };
  auto attrib_c = ChannelSelect[static_cast<int32_t>(r.channel)];
  if (r.sheet->type() == SheetType::DualIndexed) {
    attrib_c *= -1;
  }

  vertices[nv] = { a, r.left, r.top, palette_texture_index, attrib_c };
  vertices[nv + 1] = { b, r.right, r.top, palette_texture_index, attrib_c };
  vertices[nv + 2] = { c, r.right, r.bottom, palette_texture_index, attrib_c };
  vertices[nv + 3] = { d, r.left, r.bottom, palette_texture_index, attrib_c };
}

static SdlSurfacePtr CloneWith32bbpArgbPixelFormat(SDL_Surface& src) {
  SdlSurfacePtr cloned(SDL_ConvertSurfaceFormat(&src, SDL_PIXELFORMAT_ARGB8888, 0));
  if (cloned == nullptr) {
    throw Error(MSG(std::string("SDL_ConvertSurfaceFormat error: ") + SDL_GetError()));
  }
  return cloned;
}

void GraphicsUtil::FastCopyIntoSprite(std::vector<char>& dest_data,
                                      int32_t dest_stride, 
                                      const Rectangle& bounds,
                                      SDL_Surface* src) {
  SdlSurfacePtr cloned_src;
  if (src->format->format != SDL_PIXELFORMAT_ARGB8888) {
    cloned_src = CloneWith32bbpArgbPixelFormat(*src);
    src = cloned_src.get();
  }

  auto width = bounds.width;
  auto height = bounds.height;

  SDL_LockSurface(src);
  
  int32_t* c = static_cast<int32_t*>(src->pixels);
  int32_t* data = reinterpret_cast<int32_t*>(&dest_data[0]);
  auto x = bounds.x;
  auto y = bounds.y;
  for (auto j = 0; j < height; ++j) {
    for (auto i = 0; i < width; ++i) {
      Color cc(*(c + (j * src->pitch >> 2) + i));
      data[(y + j) * dest_stride + x + i] = PremultiplyAlpha(cc).ToArgb();
    }
  }

  SDL_UnlockSurface(src);
}

Color GraphicsUtil::PremultiplyAlpha(const Color& c) {
  if (c.a == 255) {
    return c;
  }
  auto a = c.a / 255.0f;
  return{
    static_cast<uint8_t>(c.r * a + 0.5f),
    static_cast<uint8_t>(c.g * a + 0.5f),
    static_cast<uint8_t>(c.b * a + 0.5f),
    static_cast<uint8_t>(c.a)
  };
}

}
