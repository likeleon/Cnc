#pragma once

#include "cnc/error.h"

namespace cnc {

enum class SeekOrigin {
  Begin = 0,
  Current = 1
};

class SegmentStream;

class CNC_API Stream {
public:
  virtual size_t Length() const = 0;
  virtual size_t Position() const = 0;
  virtual void SetPosition(size_t position) = 0;
  virtual void Seek(size_t position, SeekOrigin origin) = 0;
  
  virtual uint8_t ReadUInt8() = 0;
  virtual uint16_t ReadUInt16() = 0;
  virtual uint32_t ReadUInt32() = 0;
  virtual int32_t ReadInt32() = 0;
  virtual std::vector<char> ReadBytes(size_t count) = 0;
  virtual void ReadBytes(std::vector<char>& dest, size_t offset, size_t count) = 0;

  virtual SegmentStream* AsSegmentStream() = 0;
};

}
