#include "cnc/stdafx.h"
#include "cnc/buffer_utils.h"
#include "cnc/error.h"

namespace cnc {

template <typename T>
static T ReadBytes(const std::vector<char>& s, size_t& offset) {
  if (s.size() < start_index || sizeof(T) > s.size() - start_index) {
    throw Error(MSG("End of stream"));
  }
  T t;
  memcpy(&t, &s[start_index], sizeof(T));
  offset += sizeof(T);
  return t;
}

uint8_t BufferUtils::ReadUInt8(const std::vector<char>& s, size_t& offset) {
  return ReadBytes<uint8_t>(s, offset);
}

uint16_t BufferUtils::ReadUInt16(const std::vector<char>& s, size_t& offset) {
  return ReadBytes<uint16_t>(s, offset);
}

uint32_t BufferUtils::ReadUInt32(const std::vector<char>& s, size_t& offset) {
  return ReadBytes<uint32_t>(s, offset);
}

}
