#pragma once

#include "cnc/stream.h"

namespace cnc {

class SegmentStream : public Stream {
public:
  SegmentStream(StreamPtr stream, size_t offset, size_t count);
  
  static size_t GetOverallNestedOffset(StreamPtr stream, StreamPtr& overall_base_stream);

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
  
  SegmentStream* AsSegmentStream() { return this; }

private:
  StreamPtr base_stream_;
  size_t base_offset_ = 0;
  size_t base_count_ = 0;
};

}
