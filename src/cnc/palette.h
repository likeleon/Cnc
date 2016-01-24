#pragma once

#include "cnc/ipalette.h"
#include "cnc/palette_ptr.h"

namespace cnc {

struct Color;

class Palette {
public:
  Palette() = delete;

  static Color GetColor(const IPalette& palette, int32_t index);

  static const int32_t Size = 256;
};

class ImmutablePalette : public IPalette {
public:
  ImmutablePalette(const std::string& filename, const std::vector<int32_t>& remap_shadow);
  ImmutablePalette(const std::vector<char>& s, const std::vector<int32_t>& remap_shadow);
  ImmutablePalette(const IPalette& p);

  uint32_t operator[](int32_t index) const override;
  void CopyToArray(std::vector<char>& destination, int32_t destination_offset) const override;

private:
  void LoadFromStream(const std::vector<char>& s, const std::vector<int32_t>& remap_shadow);

  std::array<uint32_t, Palette::Size> colors_;
};

class MutablePalette : public IPalette {
public:
  MutablePalette(const IPalette& p);

  void SetFromPalette(const IPalette& p);

  uint32_t operator[](int32_t index) const override;
  uint32_t& operator[](int32_t index);
  
  void CopyToArray(std::vector<char>& destination, int32_t destination_offset) const override;

private:
  std::array<uint32_t, Palette::Size> colors_;
};

}
