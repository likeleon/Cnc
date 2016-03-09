#pragma once

#include "cnc/error.h"

namespace cnc {

enum class SeekOrigin {
  Begin = 0,
  Current = 1
};

class CNC_API Stream {
public:
  size_t length() const;
  size_t position() const;
  void SetPosition(size_t position);
  void Seek(size_t position, SeekOrigin origin);
  
  uint8_t ReadUInt8();
  uint16_t ReadUInt16();
  uint32_t ReadUInt32();
  int32_t ReadInt32();
  std::vector<char> ReadBytes(size_t count);
  void ReadBytes(std::vector<char>& dest, size_t offset, size_t count);

  std::vector<char>& buffer();

protected:
  Stream(std::vector<char>&& buffer);

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
