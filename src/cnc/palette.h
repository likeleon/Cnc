#pragma once

namespace cnc {

struct Color;

class IPalette {
public:
  virtual ~IPalette() {}

  virtual uint32_t operator()(int32_t index) const = 0;
  virtual void CopyToArray(std::vector<char>& destination, int32_t destination_offset) const = 0;
};

class Palette {
public:
  Palette() = delete;

  static Color GetColor(IPalette&, int32_t index);

  static const int32_t Size = 256;
};

}
