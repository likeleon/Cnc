#include "cnc/stdafx.h"
#include "cnc/sprite.h"
#include "cnc/sheet.h"
#include "cnc/size.h"

namespace cnc {

Sprite::Sprite(const SheetPtr& sheet,
               const Rectangle& bounds,
               TextureChannel channel)
  : Sprite(sheet, bounds, Float2::Zero, channel) {

}

Sprite::Sprite(const SheetPtr& sheet,
               const Rectangle& bounds,
               const Float2& offset,
               TextureChannel channel,
               BlendMode blend_mode)
  : sheet(sheet),
  bounds(bounds),
  offset(offset),
  channel(channel),
  size(bounds.Size()),
  blend_mode(blend_mode),
  fractional_offset(offset / size) {
  left = static_cast<float>(std::min(bounds.Left(), bounds.Right())) / sheet->size().width;
  top = static_cast<float>(std::min(bounds.Top(), bounds.Bottom())) / sheet->size().height;
  right = static_cast<float>(std::max(bounds.Left(), bounds.Right())) / sheet->size().width;
  bottom = static_cast<float>(std::max(bounds.Top(), bounds.Bottom())) / sheet->size().height;
}

}