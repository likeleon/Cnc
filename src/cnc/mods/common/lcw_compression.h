#pragma once

namespace cnc {
namespace mods {
namespace common {

class LCWCompression {
public:
  LCWCompression() = delete;

  static int32_t DecodeInto(const std::vector<char>& src, std::vector<char>& dest, int32_t src_offset = 0, bool reverse = false);
};

}
}
}
