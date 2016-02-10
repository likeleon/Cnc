#pragma once

namespace cnc {

class CNC_API BufferUtils {
public:
  static uint8_t ReadUInt8(const std::vector<char>& s, size_t& start_index);
  static uint16_t ReadUInt16(const std::vector<char>& s, size_t& start_index);
  static uint32_t ReadUInt32(const std::vector<char>& s, size_t& start_index);
  static int32_t ReadInt32(const std::vector<char>& s, size_t& start_index);
  
  static std::vector<char> ReadBytes(const std::vector<char>& s, size_t offset, size_t length);
};

}
