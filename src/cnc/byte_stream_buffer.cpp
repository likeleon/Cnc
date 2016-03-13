#include "cnc/stdafx.h"
#include "cnc/byte_stream_buffer.h"

namespace cnc {

ByteStreamBuffer::ByteStreamBuffer(std::vector<char>&& v)
  : v_(std::move(v)) {
  setg(&v_[0], &v_[0], &v_[0] + v_.size());
  setp(&v_[0], &v_[0] + v_.size());
}

ByteStreamBuffer::int_type ByteStreamBuffer::underflow() {
  if (gptr() >= egptr()) {
    return traits_type::eof();
  }
  return traits_type::to_int_type(*gptr());
}

std::streampos ByteStreamBuffer::seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode /*which*/) {
  char *p = nullptr;
  switch (way) {
  case std::ios_base::beg:
    p = eback() + off;
    break;
  case std::ios_base::cur:
    p = gptr() + off;
    break;
  case std::ios_base::end:
    p = egptr() + off;
    break;
  default:
    p = 0;
    break;
  }
  if (p >= eback() && p <= egptr()) {
    setg(eback(), p, egptr());
    return std::streampos(p - eback());
  } else {
    return -1;
  }
}

std::streampos ByteStreamBuffer::seekpos(std::streampos pos, std::ios_base::openmode /*which*/) {
  char *p = eback() + static_cast<int32_t>(pos);
  if (p >= eback() && p <= egptr()) {
    setg(eback(), p, egptr());
    return pos;
  } else {
    return -1;
  }
}

}
