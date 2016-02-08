#include "cnc/stdafx.h"
#include "cnc/sheet_builder.h"
#include "cnc/size.h"
#include "cnc/float2.h"
#include "cnc/game.h"
#include "cnc/settings.h"
#include "cnc/sheet.h"
#include "cnc/sprite_loader.h"
#include "cnc/graphics_util.h"
#include "cnc/bitmap.h"

namespace cnc {

SheetBuilder::SheetBuilder(SheetType t)
  : SheetBuilder(t, Game::settings().graphics().sheet_size) {
}

SheetBuilder::SheetBuilder(SheetType t, int32_t sheet_size)
  : SheetBuilder(t, [t, sheet_size]() { return AllocateSheet(t, sheet_size); }) {
}

SheetBuilder::SheetBuilder(SheetType t, const std::function<SheetPtr()>& allocate_sheet)
  : type_(t) {
  current_ = allocate_sheet();
  sheets_.emplace_back(current_);
  allocate_sheet_ = allocate_sheet;
}

SheetPtr SheetBuilder::AllocateSheet(SheetType type, int32_t sheet_size) {
  return std::make_shared<Sheet>(type, Size(sheet_size, sheet_size));
}

SheetType SheetBuilder::type() const {
  return type_;
}

SheetPtr SheetBuilder::current() {
  return current_;
}

Sprite SheetBuilder::Add(ISpriteFrame& frame) {
  return Add(frame.data(), frame.size(), frame.offset());
}

Sprite SheetBuilder::Add(const std::vector<char>& src, const Size& size) {
  return Add(src, size, Float2::Zero);
}

Sprite SheetBuilder::Add(const std::vector<char>& src, const Size& size, const Float2& sprite_offset) {
  if (size.width == 0 || size.height == 0) {
    return Sprite(current_, Rectangle::Empty, sprite_offset, channel_, BlendMode::Alpha);
  }

  auto sprite = Allocate(size, sprite_offset);
  GraphicsUtil::FastCopyIntoChannel(sprite, src);
  current_->CommitBufferedData();
  return sprite;
}

Sprite SheetBuilder::Add(const Bitmap& src) {
  auto sprite = Allocate(src.Size());
  GraphicsUtil::FastCopyIntoSprite(sprite, &src);
  current_->CommitBufferedData();
  return sprite;
}

Sprite SheetBuilder::Allocate(const Size& image_size) {
  return Allocate(image_size, Float2::Zero);
}

Sprite SheetBuilder::Allocate(const Size& image_size, const Float2& sprite_offset) {
  if (image_size.width + p_.x > current_->size().width) {
    p_ = { 0, p_.y + row_height_ };
    row_height_ = image_size.height;
  }

  if (image_size.height > row_height_) {
    row_height_ = image_size.height;
  }

  if (p_.y + image_size.height > current_->size().height) {
    TextureChannel next;
    if (!NextChannel(channel_, next)) {
      current_->ReleaseBuffer();
      current_ = allocate_sheet_();
      sheets_.emplace_back(current_);
      channel_ = TextureChannel::Red;
    } else {
      channel_ = next;
    }
    
    row_height_ = image_size.height;
    p_ = { 0, 0 };
  }

  Sprite sprite(current_, { p_, image_size }, sprite_offset, channel_, BlendMode::Alpha);
  p_.x += image_size.width;
  
  return sprite;
}

bool SheetBuilder::NextChannel(TextureChannel t, TextureChannel& next) {
  auto next_channel = static_cast<int32_t>(t) + static_cast<int32_t>(type_);
  if (next_channel > static_cast<int32_t>(TextureChannel::Alpha)) {
    return false;
  }
  next = static_cast<TextureChannel>(next_channel);
  return true;
}

}