#pragma once

#include <streambuf>

namespace cnc {

class ByteStreamBuffer : public std::streambuf {
public:
  explicit ByteStreamBuffer(std::vector<char>&& v);

  int_type underflow() override;
  std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
                         std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;
  std::streampos seekpos(std::streampos sp,
                         std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;
private:
  std::vector<char> v_;
};

}