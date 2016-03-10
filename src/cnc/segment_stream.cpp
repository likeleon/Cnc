#include "cnc/stdafx.h"
#include "cnc/segment_stream.h"

namespace cnc {


SegmentStream::SegmentStream(StreamPtr stream, size_t offset, size_t count)
  : base_stream_(stream), base_offset_(offset), base_count_(count) {
  stream->Seek(base_offset_, SeekOrigin::Begin);
}

size_t SegmentStream::GetOverallNestedOffset(StreamPtr stream, StreamPtr& overall_base_stream) {
  size_t offset = 0;
  overall_base_stream = stream;
  auto* segment_stream = stream->AsSegmentStream();
  if (segment_stream != nullptr) {
    offset += segment_stream->base_offset_ + GetOverallNestedOffset(segment_stream->base_stream_, overall_base_stream);
  }
  return offset;
}

size_t SegmentStream::Length() const {
  return base_count_;
}

size_t SegmentStream::Position() const {
  return base_stream_->Position() - base_offset_;
}

void SegmentStream::SetPosition(size_t position) {
  base_stream_->SetPosition(base_offset_ + position);
}

void SegmentStream::Seek(size_t position, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    base_stream_->Seek(base_offset_ + position, SeekOrigin::Begin);
    return;
  case SeekOrigin::Current:
    base_stream_->Seek(position, SeekOrigin::Current);
    return;
  default:
    throw Error(MSG("Invalid SeekOrigin enum value"));
  }
}

uint8_t SegmentStream::ReadUInt8() {
  return base_stream_->ReadUInt8();
}

uint16_t SegmentStream::ReadUInt16() {
  return base_stream_->ReadUInt16();
}

uint32_t SegmentStream::ReadUInt32() {
  return base_stream_->ReadUInt32();
}

int32_t SegmentStream::ReadInt32() {
  return base_stream_->ReadInt32();
}

std::vector<char> SegmentStream::ReadBytes(size_t count) {
  return base_stream_->ReadBytes(count);
}

void SegmentStream::ReadBytes(std::vector<char>& dest, size_t offset, size_t count) {
  base_stream_->ReadBytes(dest, offset, count);
}

}