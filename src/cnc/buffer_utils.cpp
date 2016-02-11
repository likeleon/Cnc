#include "cnc/stdafx.h"
#include "cnc/buffer_utils.h"
#include "cnc/error.h"

namespace cnc {

template <typename T>
static T Read(const std::vector<char>& s, size_t& offset) {
  if (s.size() < offset || sizeof(T) > s.size() - offset) {
    throw Error(MSG("End of stream"));
  }
  T t;
  memcpy(&t, &s[offset], sizeof(T));
  offset += sizeof(T);
  return t;
}

uint8_t BufferUtils::ReadUInt8(const std::vector<char>& s, size_t& offset) {
  return Read<uint8_t>(s, offset);
}

uint16_t BufferUtils::ReadUInt16(const std::vector<char>& s, size_t& offset) {
  return Read<uint16_t>(s, offset);
}

uint32_t BufferUtils::ReadUInt32(const std::vector<char>& s, size_t& offset) {
  return Read<uint32_t>(s, offset);
}

int32_t BufferUtils::ReadInt32(const std::vector<char>& s, size_t& offset) {
  return Read<int32_t>(s, offset);
}

std::vector<char> BufferUtils::ReadBytes(const std::vector<char>& s, size_t offset, size_t length) {
  if (s.size() < offset || length > s.size() - offset) {
    throw Error(MSG("End of stream"));
  }
  return{ s.begin() + offset, s.begin() + offset + length };
}

void BufferUtils::ReadBytes(const std::vector<char>& s, std::vector<char>& dest, size_t dest_offset, size_t src_offset, size_t length) {
  if (length < 0) {
    throw Error(MSG("Non-negative number required"));
  }

  std::copy(s.begin() + src_offset, s.begin() + src_offset + length, dest.begin() + dest_offset);
}

}
