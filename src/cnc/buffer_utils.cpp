#include "cnc/stdafx.h"
#include "cnc/buffer_utils.h"

namespace cnc {

template <class T>
static T Read(const std::vector<char>& b, size_t start_index) {
  T t;
  memcpy(&t, &v[start_index], sizeof(T));
  return t;
}

uint32_t BufferUtils::ReadUInt32(const std::vector<char>& b, size_t start_index) {
  return Read<uint32_t>(b, start_index);
}

}