#pragma once

#include "cnc/size.h"

namespace cnc {

enum class SheetType {
  Indexed = 1,
  DualIndexed = 2,
  BGRA = 4
};

class Sheet {
public:
  Sheet(SheetType type, const std::string& filename);

  const Size& size() const;

private:
  Size size_;
};

}
