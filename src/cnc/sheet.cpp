#include "cnc/stdafx.h"
#include "cnc/sheet.h"
#include "cnc/file_system.h"
#include "cnc/error.h"
#include "cnc/graphics_util.h"
#include "cnc/rectangle.h"
#include "cnc/point.h"
#include "cnc/sprite.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/bitmap.h"

namespace cnc {

Sheet::Sheet(SheetType type, const Size& size)
  : type_(type), size_(size) {
}

Sheet::Sheet(SheetType type, ITexturePtr texture)
  : type_(type), texture_(texture), size_(texture->size()) {
}

Sheet::Sheet(SheetType type, const std::vector<char>& stream) {
  Bitmap bitmap(stream);
  size_ = bitmap.Size();
  data_.resize(4 * size_.width * size_.height);
  GraphicsUtil::FastCopyIntoSprite(GetData(), size_.width, &bitmap);

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

void Sheet::CommitBufferedData() {
  if (!Buffered()) {
    throw Error(MSG("This sheet is unbuffered. You cannot call CommitBufferedData on an unbuffered sheet. "
                    "If you need to completely replace the texture data you should set data into the texture directly. "
                    "If you need to make only small changes to the texture data consider creating a buffered sheet instead."));
  }

  dirty_ = true;
}

void Sheet::ReleaseBuffer() {
  if (!Buffered()) {
    return;
  }
  dirty_ = true;
  release_buffer_on_commit_ = true;
}

}