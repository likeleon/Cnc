#include "cnc/stdafx.h"
#include "cnc/graphics_util.h"
#include "cnc/surface_ptr.h"
#include "cnc/rectangle.h"
#include "cnc/error.h"
#include "cnc/color.h"

namespace cnc {

static SdlSurfacePtr CloneWith32bbpArgbPixelFormat(SDL_Surface& src) {
  SdlSurfacePtr cloned(SDL_ConvertSurfaceFormat(&src, SDL_PIXELFORMAT_ARGB8888, 0));
  if (cloned == nullptr) {
    throw Error(MSG(std::string("SDL_ConvertSurfaceFormat error: ") + SDL_GetError()));
  }
  return cloned;
}

void GraphicsUtil::FastCopyIntoSprite(std::string& dest_data,
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
