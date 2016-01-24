#pragma once

namespace cnc {

class IPalette {
public:
  virtual ~IPalette() {}

  virtual uint32_t operator[](int32_t index) const = 0;
  virtual void CopyToArray(std::vector<char>& destination, int32_t destination_offset) const = 0;
};

}
