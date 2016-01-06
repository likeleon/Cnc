#pragma once

#include "cnc/sprite.h"
#include "cnc/point.h"
#include "cnc/sheet_ptr.h"

namespace cnc {

struct Float2;
struct Size;

enum class SheetType {
  Indexed = 1,
  DualIndexed = 2,
  BGRA = 4
};

class SheetBuilder {
public:
  SheetBuilder(SheetType t);
  SheetBuilder(SheetType t, int32_t sheet_size);
  SheetBuilder(SheetType t, const std::function<SheetPtr()>& allocate_sheet);

  static SheetPtr AllocateSheet(SheetType t, int32_t sheet_size);

  SheetType type() const;

  Sprite Allocate(const Size& image_size);
  Sprite Allocate(const Size& image_size, const Float2& sprite_offset);

private:
  bool NextChannel(TextureChannel t, TextureChannel& next);

  SheetType type_;
  std::vector<SheetPtr> sheets_;
  std::function<SheetPtr()> allocate_sheet_;
  SheetPtr current_;
  TextureChannel channel_ = TextureChannel::Red;
  int32_t row_height_ = 0;
  Point p_{ 0, 0 };
};

}
