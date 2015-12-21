#pragma once

#include "sheet_ptr.h"
#include "igraphics_device.h"
#include "rectangle.h"
#include "float2.h"

namespace cnc {

enum class TextureChannel {
  Red,
  Green,
  Blue,
  Alpha
};

struct CNC_API Sprite {
  Sprite(const SheetPtr& sheet,
         const Rectangle& bounds,
         TextureChannel channel);

  Sprite(const SheetPtr& sheet,
         const Rectangle& bounds,
         const Float2& offset,
         TextureChannel channel,
         BlendMode blend_mode = BlendMode::Alpha);

  Rectangle bounds;
  SheetPtr sheet;
  BlendMode blend_mode;
  TextureChannel channel;
  Float2 size;
  Float2 offset;
  Float2 fractional_offset;
  float top;
  float left;
  float bottom;
  float right;
};

}
