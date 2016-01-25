#include "cnc/stdafx.h"
#include "cnc/hardware_palette.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/igraphics_device.h"
#include "cnc/math_utils.h"
#include "cnc/palette.h"

namespace cnc {

HardwarePalette::HardwarePalette()
  : texture_(Game::renderer()->device().CreateTexture()) {
}

bool HardwarePalette::Contais(const std::string& name) const {
  return modifiable_palettes_.find(name) != modifiable_palettes_.end() ||
    palettes_.find(name) != palettes_.end();
}

IPalettePtr HardwarePalette::GetPalette(const std::string& name) const {
  auto it1 = modifiable_palettes_.find(name);
  if (it1 != modifiable_palettes_.end()) {
    return it1->second;
  }
  auto it2 = palettes_.find(name);
  if (it2 != palettes_.end()) {
    return it2->second;
  }
  throw Error(MSG("Palette '" + name + "' does not exist"));
}

int32_t HardwarePalette::GetPaletteIndex(const std::string& name) const {
  auto iter = indices_.find(name);
  if (iter == indices_.end()) {
    throw Error(MSG("Palette '" + name + "' does not exist"));
  }
  return iter->second;
}

void HardwarePalette::AddPalette(const std::string& name, const ImmutablePalettePtr& p, bool allow_modifiers) {
  if (palettes_.find(name) != palettes_.end()) {
    throw Error(MSG("Palette " + name + " has already been defined"));
  }

  auto index = static_cast<int32_t>(palettes_.size());
  indices_.emplace(name, index);
  palettes_.emplace(name, p);

  if (static_cast<int32_t>(palettes_.size()) > height_) {
    height_ = NextPowerOf2(palettes_.size());
    buffer_.resize(height_ * Palette::Size * 4);
  }

  if (allow_modifiers) {
    modifiable_palettes_.emplace(name, std::make_shared<MutablePalette>(*p));
  } else {
    CopyPaletteToBuffer(index, *p);
  }
}

void HardwarePalette::Initialize() {
  CopyModifiablePalettsToBuffer();
  CopyBufferToTexture();
}

void HardwarePalette::CopyPaletteToBuffer(int32_t index, const IPalette& p) {
  p.CopyToArray(buffer_, index * Palette::Size);
}

void HardwarePalette::CopyModifiablePalettsToBuffer() {
  for (const auto& kvp : modifiable_palettes_) {
    CopyPaletteToBuffer(indices_[kvp.first], *kvp.second);
  }
}

void HardwarePalette::CopyBufferToTexture() {
  texture_->SetData(buffer_, Palette::Size, height_);
}

const ITexturePtr& HardwarePalette::texture() const {
  return texture_;
}

int32_t HardwarePalette::height() const {
  return height_;
}

}