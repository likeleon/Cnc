#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/rle_zeros_compression.h"
#include "cnc/mods/common/fast_byte_reader.h"

namespace cnc {
namespace mods {
namespace common {

void RLEZerosCompression::DecodeInto(const std::vector<char>& src, std::vector<char>& dest, int32_t dest_index) {
  FastByteReader r(src, 0);
  while (!r.Done()) {
    auto cmd = r.ReadByte();
    if (cmd == 0) {
      auto count = r.ReadByte();
      while (count-- > 0) {
        dest[dest_index++] = 0;
      }
    } else {
      dest[dest_index++] = cmd;
    }
  }
}

}
}
}
