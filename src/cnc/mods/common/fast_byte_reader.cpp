#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/fast_byte_reader.h"

namespace cnc {
namespace mods {
namespace common {

FastByteReader::FastByteReader(const std::vector<char>& src, int32_t offset)
  : src_(src), offset_(offset) {
}

bool FastByteReader::Done() {
  return static_cast<size_t>(offset_) >= src_.size();
}

uint8_t FastByteReader::ReadByte() {
  return src_[offset_++];
}

int32_t FastByteReader::ReadWord() {
  auto x = ReadByte();
  return x | (ReadByte() << 8);
}

void FastByteReader::CopyTo(std::vector<char>& dest, int32_t offset, int32_t count) {
  std::copy(src_.begin() + offset_, src_.begin() + offset_ + count, dest.begin() + offset);
  offset_ += count;
}

int32_t FastByteReader::Remaining() const {
  return src_.size() - offset_;
}

}
}
}
