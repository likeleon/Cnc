#pragma once

#include "cnc/action.h"

namespace cnc {
namespace mods {
namespace common {

class InstallUtils {
public:
  static bool ExtractZip(const std::string& zip_file, const std::string& dest, Action<std::string> on_progress, Action<std::string> on_error);
};

}
}
}
