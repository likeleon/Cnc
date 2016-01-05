#pragma once

#include "cnc/sprite.h"

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

  SheetType type() const;

  Sprite Allocate(const Size& image_size) const;
  Sprite Allocate(const Size& image_size, const Float2& sprite_offset) const;

private:
  SheetType type_;
};

}
