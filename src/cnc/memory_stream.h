#pragma once

#include "cnc/stream.h"

namespace cnc {

class MemoryStream : public Stream {
public:
  size_t Length() const override;
  size_t Position() const override;
  void SetPosition(size_t position) override;
  void Seek(size_t position, SeekOrigin origin) override;

  uint8_t ReadUInt8() override;
  uint16_t ReadUInt16() override;
  uint32_t ReadUInt32() override;
  int32_t ReadInt32() override;
  std::vector<char> ReadBytes(size_t count) override;
  void ReadBytes(std::vector<char>& dest, size_t offset, size_t count) override;
  
  SegmentStream* AsSegmentStream() { return nullptr; }
  std::vector<char>& buffer();

protected:
  MemoryStream(std::vector<char>&& buffer);

private:
  template <typename T>
  T Read() {
    if (sizeof(T) > length_ - position_) {
      position_ = length_;
      throw Error(MSG("End of stream"));
    }
    T t;
    memcpy(&t, &buffer_[position_], sizeof(T));
    position_ += sizeof(T);
    return t;
  }

  std::vector<char> buffer_;
  size_t length_ = 0;
  size_t position_ = 0;
};

}
