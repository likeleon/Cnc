#pragma once

#include "cnc/manifest.h"

namespace cnc {

class ModData {
public:
  ModData(const std::string& mod, bool use_load_screen = false);

private:
  Manifest manifest_;
};

}