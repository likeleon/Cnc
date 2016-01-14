#pragma once

namespace cnc {

class BufferUtils {
public:
  BufferUtils() = delete;

  static uint32_t ReadUInt32(const std::vector<char>& b, size_t start_index);
};

}
