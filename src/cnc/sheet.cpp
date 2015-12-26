#include "cnc/stdafx.h"
#include "cnc/sheet.h"
#include "cnc/file_system.h"
#include "cnc/error.h"
#include "cnc/graphics_util.h"
#include "cnc/rectangle.h"
#include "cnc/point.h"
#include "cnc/sprite.h"
#include "cnc/surface_ptr.h"
#include "cnc/game.h"
#include "cnc/renderer.h"

namespace cnc {


Sheet::Sheet(SheetType type, ITexturePtr texture)
  : type_(type), texture_(texture), size_(texture->size()) {
}

static SdlSurfacePtr SurfaceFromStream(const std::vector<char>& stream) {
  auto* rw = SDL_RWFromMem(const_cast<char*>(stream.data()), static_cast<int32_t>(stream.size()));
  SdlSurfacePtr surface(IMG_Load_RW(rw, 0));
  SDL_RWclose(rw);
  if (surface == nullptr) {
    throw Error(MSG("Cannot load image"));
  }
  return surface;
}

Sheet::Sheet(SheetType type, const std::vector<char>& stream) {
  auto surface = SurfaceFromStream(stream);
  size_ = { surface->w, surface->h };
  data_.resize(4 * size_.width * size_.height);
  GraphicsUtil::FastCopyIntoSprite(GetData(), size_.width, { Point::Zero, size_ }, surface.get());
  surface.reset();

  type_ = type;
  ReleaseBuffer();
}

SheetType Sheet::type() const {
  return type_;
}

const Size& Sheet::size() const {
  return size_;
}

bool Sheet::Buffered() const {
  return !data_.empty() || texture_ == nullptr;
}

std::vector<char>& Sheet::GetData() {
  CreateBuffer();
  return data_;
}

ITexturePtr Sheet::GetTexture() {
  if (texture_ == nullptr) {
    texture_ = Game::renderer()->device().CreateTexture();
    dirty_ = true;
  }

  if (!data_.empty() && dirty_) {
    texture_->SetData(data_, size_.width, size_.height);
    dirty_ = false;
    if (release_buffer_on_commit_) {
      data_.clear();
    }
  }

  return texture_;
}

void Sheet::CreateBuffer() {
  if (!data_.empty()) {
    return;
  }
  if (texture_ == nullptr) {
    data_.resize(4 * size_.width * size_.height);
  } else {
    data_ = texture_->GetData();
  }
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