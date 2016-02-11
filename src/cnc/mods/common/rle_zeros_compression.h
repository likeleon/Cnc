#pragma once

namespace cnc {
namespace mods {
namespace common {

class RLEZerosCompression {
public:
  RLEZerosCompression() = delete;

  static void DecodeInto(const std::vector<char>& src, std::vector<char>& dest, int32_t dest_index);
};

}
}
}
