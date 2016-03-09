#include "cnc/stdafx.h"
#include "cnc/bitmap.h"
#include "cnc/error.h"
#include "cnc/size.h"
#include "cnc/rectangle.h"
#include "cnc/point.h"
#include "cnc/file.h"
#include "cnc/stream.h"

namespace cnc {

static SDL_Surface_UniquePtr SurfaceFromStream(StreamPtr stream) {
  auto* rw = SDL_RWFromMem(const_cast<char*>(&stream->buffer()[0]), static_cast<int32_t>(stream->length()));
  auto surface = IMG_Load_RW(rw, 0);
  SDL_RWclose(rw);
  if (surface == nullptr) {
    throw Error(MSG("Cannot load image"));
  }
  return SDL_Surface_UniquePtr(surface);
}

Bitmap::Bitmap(const std::string& filename) {
  surface_ = SurfaceFromStream(File::OpenRead(filename));
}

Bitmap::Bitmap(StreamPtr stream) {
  surface_ = SurfaceFromStream(stream);
}

Bitmap::Bitmap(SDL_Surface_UniquePtr surface)
: surface_(std::move(surface)) {
}

Size Bitmap::Size() const {
  return{ surface_->w, surface_->h };
}

Rectangle Bitmap::Bounds() const {
  return{ Point::Zero, Size() };
}

SDL_Surface& Bitmap::surface() const {
  return *surface_;
}

}