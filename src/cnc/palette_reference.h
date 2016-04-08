#pragma once

#include "cnc/ipalette.h"

namespace cnc {

class HardwarePalette;

class PaletteReference {
public:
  PaletteReference(const std::string& name, int32_t index, const IPalettePtr& palette, const HardwarePalette& hardware_palette);

  void set_palette(IPalettePtr palette) { palette_ = palette; }
  const IPalette& palette() const { return *palette_; }
  float TextureIndex() const;
  float TextureMidIndex() const;

private:
  std::string name_;
  IPalettePtr palette_;
  float index_;
  const HardwarePalette& hardware_palette_;
};

}
