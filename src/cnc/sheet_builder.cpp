#include "cnc/stdafx.h"
#include "cnc/sheet_builder.h"

namespace cnc {

SheetBuilder::SheetBuilder(SheetType t)
  : type_(t) {
}

SheetType SheetBuilder::type() const {
  return type_;
}

}