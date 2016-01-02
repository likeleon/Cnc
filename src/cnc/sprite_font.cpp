#include "cnc/stdafx.h"
#include "cnc/sprite_font.h"
#include "cnc/sheet_builder.h"
#include "cnc/error.h"

namespace cnc {

SpriteFont::SpriteFont(const std::string& /*name*/, int32_t size, SheetBuilder& builder)
  : size_(size), builder_(builder) {
  if (builder_.type() != SheetType::BGRA) {
    throw Error(MSG("The sheet builder must create BGRA sheets"));
  }

  // TODO: TTF
}

}