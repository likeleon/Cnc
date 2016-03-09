#include "cnc/stdafx.h"
#include "cnc/stream.h"
#include "cnc/error.h"

namespace cnc {

Stream::Stream(std::vector<char>&& buffer)
  : buffer_(std::move(buffer)) {
  length_ = buffer_.size();
}

size_t Stream::length() const {
  return length_;
}

size_t Stream::position() const {
  return position_;
}

void Stream::SetPosition(size_t position) {
  Seek(position, SeekOrigin::Begin);
}

void Stream::Seek(size_t position, SeekOrigin origin) {
  size_t new_position = position;
  if (origin == SeekOrigin::Current) {
    new_position += position_;
  }
  if (new_position > length_) {
    throw Error(MSG("new position too large"));
  }
  position_ = new_position;
}

std::vector<char>& Stream::buffer() {
  return buffer_;
}

uint8_t Stream::ReadUInt8() {
  return Read<uint8_t>();
}

uint16_t Stream::ReadUInt16() {
  return Read<uint16_t>();
}

uint32_t Stream::ReadUInt32() {
  return Read<uint32_t>();
}

int32_t Stream::ReadInt32() {
  return Read<int32_t>();
}

std::vector<char> Stream::ReadBytes(size_t count) {
  if (count > length_ - position_) {
    position_ = length_;
    throw Error(MSG("End of stream"));
  }
  return{ buffer_.begin() + position_, buffer_.begin() + position_ + count };
}

void Stream::ReadBytes(std::vector<char>& dest, size_t offset, size_t count) {
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