#pragma once

#include "cnc/size.h"

namespace cnc {

enum class SheetType {
  Indexed = 1,
  DualIndexed = 2,
  BGRA = 4
};

class CNC_API Sheet {
public:
  Sheet(SheetType type, const std::string& stream);

  const Size& size() const;

private:
  Size size_;
};

}
