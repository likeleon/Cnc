#pragma once

#include "cnc/itexture.h"

namespace cnc {

class HardwarePalette {
public:
  HardwarePalette();

private:
  ITexturePtr texture_;
};

}
