#include "cnc/stdafx.h"
#include "cnc/graphics_util.h"
#include "cnc/rectangle.h"
#include "cnc/error.h"
#include "cnc/color.h"
#include "cnc/float2.h"
#include "cnc/sprite.h"
#include "cnc/sheet.h"
#include "cnc/vertex.h"
#include "cnc/bitmap.h"

namespace cnc {
  
static const std::array<int32_t, 4> ChannelMasks = { 2, 1, 0, 3 };
static const std::array<float, 4> ChannelSelect = { 0.2f, 0.4f, 0.6f, 0.8f };

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
  auto attrib_c = ChannelSelect[static_cast<int32_t>(r.channel)];
  if (r.sheet->type() == SheetType::DualIndexed) {
    attrib_c *= -1;
  }

  vertices[nv] = { a, r.left, r.top, palette_texture_index, attrib_c };
  vertices[nv + 1] = { b, r.right, r.top, palette_texture_index, attrib_c };
  vertices[nv + 2] = { c, r.right, r.bottom, palette_texture_index, attrib_c };
  vertices[nv + 3] = { c, r.right, r.bottom, palette_texture_index, attrib_c };
  vertices[nv + 4] = { d, r.left, r.bottom, palette_texture_index, attrib_c };
  vertices[nv + 5] = { a, r.left, r.top, palette_texture_index, attrib_c };
}

void GraphicsUtil::FastCopyIntoChannel(Sprite& dest, const std::vector<char>& src) {
  return FastCopyIntoChannel(dest, 0, src);
}

void GraphicsUtil::FastCopyIntoChannel(Sprite& dest, int32_t channel_offset, const std::vector<char>& src) {
  auto& data = dest.sheet->GetData();
  auto src_stride = dest.bounds.width;
  auto dest_stride = dest.sheet->size().width * 4;
  auto dest_offset = dest_stride * dest.bounds.Top() + 
    dest.bounds.Left() * 4 + 
    ChannelMasks[static_cast<int32_t>(dest.channel) + channel_offset];
  auto dest_skip = dest_stride - 4 * src_stride;
  auto height = dest.bounds.height;

  auto src_offset = 0;
  for (auto j = 0; j < height; ++j) {
    for (auto i = 0; i < src_stride; ++i, ++src_offset) {
      data[dest_offset] = src[src_offset];
      dest_offset += 4;
    }
    dest_offset += dest_skip;
  }
}

static BitmapUniquePtr CloneWith32bbpArgbPixelFormat(const Bitmap& src) {
  SDL_Surface_UniquePtr cloned(SDL_ConvertSurfaceFormat(&src.surface(), SDL_PIXELFORMAT_ARGB8888, 0));
  if (cloned == nullptr) {
    throw Error(MSG(std::string("SDL_ConvertSurfaceFormat error: ") + SDL_GetError()));
  }
  return std::make_unique<Bitmap>(std::move(cloned));
}

void GraphicsUtil::FastCopyIntoSprite(Sprite& dest, const Bitmap* src) {
  FastCopyIntoSprite(dest.sheet->GetData(), dest.bounds, dest.sheet->size().width, src);
}

void GraphicsUtil::FastCopyIntoSprite(std::vector<char>& dest_data,
                                      const Rectangle& dest_bounds,
                                      int32_t dest_stride,
                                      const Bitmap* src) {
  BitmapUniquePtr cloned_src;
  if (src->surface().format->format != SDL_PIXELFORMAT_ARGB8888) {
    cloned_src = CloneWith32bbpArgbPixelFormat(*src);
    src = cloned_src.get();
  }

  auto width = dest_bounds.width;
  auto height = dest_bounds.height;

  SDL_LockSurface(&src->surface());

  int32_t* c = static_cast<int32_t*>(src->surface().pixels);
  int32_t* data = reinterpret_cast<int32_t*>(&dest_data[0]);
  auto x = dest_bounds.x;
  auto y = dest_bounds.y;
  for (auto j = 0; j < height; ++j) {
    for (auto i = 0; i < width; ++i) {
      Color cc(*(c + (j * src->surface().pitch >> 2) + i));
      data[(y + j) * dest_stride + x + i] = PremultiplyAlpha(cc).ToArgb();
    }
  }

  SDL_UnlockSurface(&src->surface());
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
