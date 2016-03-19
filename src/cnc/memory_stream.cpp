#include "cnc/stdafx.h"
#include "cnc/memory_stream.h"
#include "cnc/error.h"

namespace cnc {

MemoryStream::MemoryStream(std::vector<char>&& buffer)
  : buffer_(std::move(buffer)) {
  length_ = buffer_.size();
}

size_t MemoryStream::Length() const {
  return length_;
}

size_t MemoryStream::Position() const {
  return position_;
}

void MemoryStream::SetPosition(size_t position) {
  Seek(position, SeekOrigin::Begin);
}

void MemoryStream::Seek(size_t position, SeekOrigin origin) {
  size_t new_position = position;
  if (origin == SeekOrigin::Current) {
    new_position += position_;
  }
  if (new_position > length_) {
    throw Error(MSG("new position too large"));
  }
  position_ = new_position;
}

std::vector<char>& MemoryStream::buffer() {
  return buffer_;
}

uint8_t MemoryStream::ReadUInt8() {
  return Read<uint8_t>();
}

uint16_t MemoryStream::ReadUInt16() {
  return Read<uint16_t>();
}

uint32_t MemoryStream::ReadUInt32() {
  return Read<uint32_t>();
}

int32_t MemoryStream::ReadInt32() {
  return Read<int32_t>();
}

std::vector<char> MemoryStream::ReadBytes(size_t count) {
  if (count > length_ - position_) {
    position_ = length_;
    throw Error(MSG("End of stream"));
  }
  return{ buffer_.begin() + position_, buffer_.begin() + position_ + count };
}

void MemoryStream::ReadBytes(std::vector<char>& dest, size_t offset, size_t count) {
  if (position_ + count > length_) {
    position_ = length_;
    throw Error(MSG("count too large"));
  }
  if (offset + count > dest.size()) {
    throw Error(MSG("dest size too small"));
  }
  std::copy(buffer_.begin() + position_, buffer_.begin() + position_ + count, dest.begin() + offset);

}

}