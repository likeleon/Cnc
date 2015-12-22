#include "cnc/stdafx.h"
#include "cnc/sheet.h"
#include "cnc/file_system.h"
#include "cnc/error.h"
#include "cnc/graphics_util.h"
#include "cnc/rectangle.h"
#include "cnc/point.h"
#include "cnc/sprite.h"
#include "cnc/surface_ptr.h"

namespace cnc {

static SdlSurfacePtr SurfaceFromStream(const std::string& stream) {
  auto* rw = SDL_RWFromMem(const_cast<char*>(stream.data()), static_cast<int32_t>(stream.size()));
  SdlSurfacePtr surface(IMG_Load_RW(rw, 0));
  SDL_RWclose(rw);
  if (surface == nullptr) {
    throw Error(MSG("Cannot load image"));
  }
  return surface;
}

Sheet::Sheet(SheetType type, const std::string& stream) {
  auto surface = SurfaceFromStream(stream);
  size_ = { surface->w, surface->h };
  data_.resize(4 * size_.width * size_.height);
  GraphicsUtil::FastCopyIntoSprite(GetData(), size_.width, { Point::Zero, size_ }, surface.get());
  surface.reset();

  type_ = type;
  ReleaseBuffer();
}

const Size& Sheet::size() const {
  return size_;
}

bool Sheet::Buffered() const {
  return !data_.empty();
}

std::string& Sheet::GetData() {
  CreateBuffer();
  return data_;
}

void Sheet::CreateBuffer() {
  if (!data_.empty()) {
    return;
  }
  data_.resize(4 * size_.width * size_.height);
  release_buffer_on_commit_ = false;
}

void Sheet::ReleaseBuffer() {
  if (!Buffered()) {
    return;
  }
  dirty_ = true;
  release_buffer_on_commit_ = true;
}

}