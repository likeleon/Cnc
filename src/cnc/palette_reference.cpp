#include "cnc/stdafx.h"
#include "cnc/palette_reference.h"
#include "cnc/hardware_palette.h"

namespace cnc {

PaletteReference::PaletteReference(const std::string& name, int32_t index, const IPalettePtr& palette, const HardwarePalette& hardware_palette)
  : name_(name), palette_(palette), index_(static_cast<float>(index)), hardware_palette_(hardware_palette) {
}

float PaletteReference::TextureIndex() const {
  return index_ / hardware_palette_.height();
}

float PaletteReference::TextureMidIndex() const {
  return (index_ + 0.5f) / hardware_palette_.height();
}

}