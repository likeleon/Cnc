#pragma once

#include "cnc/itexture.h"
#include "cnc/palette_ptr.h"
#include "cnc/ipalette.h"

namespace cnc {

class HardwarePalette {
public:
  HardwarePalette();

  void AddPalette(const std::string& name, const ImmutablePalettePtr& p, bool allow_modifiers);
  void Initialize();
  
  bool Contais(const std::string& name) const;
  IPalettePtr GetPalette(const std::string& name) const;
  int32_t GetPaletteIndex(const std::string& name) const;
  const ITexturePtr& texture() const;
  int32_t height() const;

private:
  void CopyPaletteToBuffer(int32_t index, const IPalette& p);
  void CopyModifiablePalettsToBuffer();
  void CopyBufferToTexture();

  ITexturePtr texture_;
  std::map<std::string, ImmutablePalettePtr> palettes_;
  std::map<std::string, MutablePalettePtr> modifiable_palettes_;
  std::map<std::string, int32_t> indices_;
  int32_t height_ = 0;
  std::vector<char> buffer_;
};

}
