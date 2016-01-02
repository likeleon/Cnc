#pragma once

namespace cnc {

enum class SheetType {
  Indexed = 1,
  DualIndexed = 2,
  BGRA = 4
};

class SheetBuilder {
public:
  SheetBuilder(SheetType t);

  SheetType type() const;

private:
  SheetType type_;
};

}
