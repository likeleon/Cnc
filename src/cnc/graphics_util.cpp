#include "cnc/stdafx.h"
#include "cnc/graphics_util.h"
#include "cnc/surface_ptr.h"
#include "cnc/rectangle.h"

namespace cnc {

static SdlSurfacePtr CloneWith32bbpArgbPixelFormat(const SDL_Surface& src) {
}

void GraphicsUtil::FastCopyIntoSprite(std::string& dest_data,
                                      int32_t dest_stride, 
                                      const Rectangle& bounds,
                                      SDL_Surface* src) {
  SdlSurfacePtr cloned_src;
  if (src->format->format != SDL_PIXELFORMAT_ARGB4444) {
    cloned_src = CloneWith32bbpArgbPixelFormat(*src);
    src = cloned_src.get();
  }

  auto width = bounds.width;
  auto height = bounds.height;

  SDL_LockSurface(src);
}

}
