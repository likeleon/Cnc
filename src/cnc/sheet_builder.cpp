#include "cnc/stdafx.h"
#include "cnc/sheet_builder.h"
#include "cnc/float2.h"

namespace cnc {

SheetBuilder::SheetBuilder(SheetType t)
  : type_(t) {
}

SheetType SheetBuilder::type() const {
  return type_;
}

Sprite SheetBuilder::Allocate(const Size& image_size) const {
  return Allocate(image_size, Float2::Zero);
}

Sprite SheetBuilder::Allocate(const Size& /*image_size*/, const Float2& /*sprite_offset*/) const {
  return Sprite();
}

}