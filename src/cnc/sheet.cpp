#include "cnc/stdafx.h"
#include "cnc/sheet.h"
#include "cnc/global_file_system.h"

namespace cnc {

Sheet::Sheet(SheetType type, const std::string& filename) {
  auto stream = GlobalFileSystem::Open(filename);
}

}