#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/lcw_compression.h"
#include "cnc/error.h"
#include "cnc/mods/common/fast_byte_reader.h"

namespace cnc {
namespace mods {
namespace common {

static void ReplicatePrevious(std::vector<char>& dest, int32_t dest_index, int32_t src_index, int32_t count) {
  if (src_index > dest_index) {
    std::ostringstream oss;
    oss << "src_index > dest_index " << src_index << " " << dest_index;
    throw Error(MSG(oss.str()));
  }

  if (dest_index - src_index == 1) {
    for (auto i = 0; i < count; ++i) {
      dest[dest_index + i] = dest[dest_index - 1];
    }
  } else {
    for (auto i = 0; i < count; ++i) {
      dest[dest_index + i] = dest[src_index + i];
    }
  }
}

int32_t LCWCompression::DecodeInto(const std::vector<char>& src, std::vector<char>& dest, int32_t src_offset, bool reverse) {
  FastByteReader ctx(src, src_offset);
  auto dest_index = 0;
  while (true) {
    auto i = ctx.ReadByte();
    if ((i & 0x80) == 0) {
      auto second_byte = ctx.ReadByte();
      size_t count = ((i & 0x70) >> 4) + 3;
      auto rpos = ((i & 0xf) << 8) + second_byte;

      if (dest_index + count > dest.size()) {
        return dest_index;
      }

      ReplicatePrevious(dest, dest_index, dest_index - rpos, count);
      dest_index += count;
    } else if ((i & 0x40) == 0) {
      auto count = i & 0x3F;
      if (count == 0) {
        return dest_index;
      }

      ctx.CopyTo(dest, dest_index, count);
      dest_index += count;
    } else {
      auto count3 = i & 0x3F;
      if (count3 == 0x3E) {
        auto count = ctx.ReadWord();
        auto color = ctx.ReadByte();

        for (auto end = dest_index + count; dest_index < end; ++dest_index) {
          dest[dest_index] = color;
        }
      } else if (count3 == 0x3F) {
        auto count = ctx.ReadWord();
        auto src_index = reverse ? dest_index - ctx.ReadWord() : ctx.ReadWord();
        if (src_index >= dest_index) {
          std::ostringstream oss;
          oss << "src_index > dest_index " << src_index << " " << dest_index;
          throw Error(MSG(oss.str()));
        }

        for (auto end = dest_index + count; dest_index < end; ++dest_index) {
          dest[dest_index] = dest[src_index++];
        }
      } else {
        auto count = count3 + 3;
        auto src_index = reverse ? dest_index - ctx.ReadWord() : ctx.ReadWord();
        if (src_index >= dest_index) {
          std::ostringstream oss;
          oss << "src_index > dest_index " << src_index << " " << dest_index;
          throw Error(MSG(oss.str()));
        }

        for (auto end = dest_index + count; dest_index < end; ++dest_index) {
          dest[dest_index] = dest[src_index++];
        }
      }
    }
  }
}

}
}
}
