#include "cnc/stdafx.h"
#include "cnc/sheet.h"
#include "cnc/file_system.h"

namespace cnc {

Sheet::Sheet(SheetType /*type*/, const std::string& /*stream*/) {
}

const Size& Sheet::size() const {
  return size_;
}

}