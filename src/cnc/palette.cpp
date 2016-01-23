#include "cnc/stdafx.h"
#include "cnc/palette.h"
#include "cnc/color.h"
#include "cnc/file.h"
#include "cnc/buffer_utils.h"

namespace cnc {

Color Palette::GetColor(const IPalette& palette, int32_t index) {
  return Color(static_cast<int32_t>(palette[index]));
}

ImmutablePalette::ImmutablePalette(const std::string& filename, const std::vector<int32_t>& remap_shadow) {
  LoadFromStream(File::ReadAllBytes(filename), remap_shadow);
}

ImmutablePalette::ImmutablePalette(const std::vector<char>& s, const std::vector<int32_t>& remap_shadow) {
  LoadFromStream(s, remap_shadow);
}

ImmutablePalette::ImmutablePalette(const IPalette& p) {
  for (auto i = 0; i < Palette::Size; ++i) {
    colors_[i] = p[i];
  }
}

void ImmutablePalette::LoadFromStream(const std::vector<char>& s, const std::vector<int32_t>& remap_shadow) {
  uint32_t offset = 0;
  for (auto i = 0; i < Palette::Size; ++i) {
    auto r = static_cast<uint8_t>(BufferUtils::ReadUInt8(s, offset) << 2);
    auto g = static_cast<uint8_t>(BufferUtils::ReadUInt8(s, offset) << 2);
    auto b = static_cast<uint8_t>(BufferUtils::ReadUInt8(s, offset) << 2);
    colors_[i] = static_cast<uint32_t>((255 << 24) | (r << 16) | (g << 8) | b);
  }
  colors_[0] = 0; // convert black background -> transparency
  for (int32_t i : remap_shadow) {
    colors_[i] = 140u << 24;
  }
}

uint32_t ImmutablePalette::operator[](int32_t index) const {
  return colors_[index];
}

void ImmutablePalette::CopyToArray(std::vector<char>& destination, int32_t destination_offset) const {
  destination.resize(destination_offset * 4 + Palette::Size * 4);
  memcpy(&destination[destination_offset * 4], &colors_[0], Palette::Size * 4);
}

MutablePalette::MutablePalette(const IPalette& p) {
  SetFromPalette(p);
}

void MutablePalette::SetFromPalette(const IPalette& p) {
  std::vector<char> buffer;
  p.CopyToArray(buffer, 0);
  memcpy(&colors_[0], &buffer[0], buffer.size());
}

uint32_t MutablePalette::operator[](int32_t index) const {
  return colors_[index];
}

uint32_t& MutablePalette::operator[](int32_t index) {
  return colors_[index];
}

void MutablePalette::CopyToArray(std::vector<char>& destination, int32_t destination_offset) const {
  destination.resize(destination_offset * 4 + Palette::Size * 4);
  memcpy(&destination[destination_offset * 4], &colors_[0], Palette::Size * 4);
}

}
