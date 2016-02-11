#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/shp_ts_loader.h"
#include "cnc/buffer_utils.h"
#include "cnc/size.h"
#include "cnc/float2.h"
#include "cnc/mods/common/rle_zeros_compression.h"

namespace cnc {
namespace mods {
namespace common {

static bool IsShpTS(const std::vector<char>& s) {
  size_t offset = 0;
  
  if (BufferUtils::ReadUInt16(s, offset) != 0) {
    return false;
  }

  offset += 4;
  auto image_count = BufferUtils::ReadUInt16(s, offset);
  if (offset + 24 * image_count > s.size()) {
    return false;
  }

  offset += 4;
  uint16_t w = 0, h = 0, f = 0;
  uint8_t type = 0;
  do {
    w = BufferUtils::ReadUInt16(s, offset);
    h = BufferUtils::ReadUInt16(s, offset);
    type = BufferUtils::ReadUInt8(s, offset);
  } while (w == 0 && h == 0 && f++ < image_count);

  return type < 4;
}

class ShpTSFrame : public ISpriteFrame {
public:
  ShpTSFrame(const std::vector<char>& s, size_t& offset, const Size& frame_size) {
    auto x = BufferUtils::ReadUInt16(s, offset);
    auto y = BufferUtils::ReadUInt16(s, offset);
    auto width = BufferUtils::ReadUInt16(s, offset);
    auto height = BufferUtils::ReadUInt16(s, offset);

    auto data_width = width;
    auto data_height = height;
    if (data_width % 2 == 1) {
      data_width += 1;
    }
    
    if (data_height % 2 == 1) {
      data_height += 1;
    }

    offset_ = { x + (data_width - frame_size.width) / 2, y + (data_height - frame_size.height) / 2 };
    size_ = { data_width, data_height };
    frame_size_ = frame_size;

    format_ = BufferUtils::ReadUInt8(s, offset);
    offset += 11;
    file_offset_ = BufferUtils::ReadUInt32(s, offset);

    if (file_offset_ == 0) {
      return;
    }

    auto start = offset;
    offset = file_offset_;

    data_.resize(data_width * data_height);

    if (format_ == 3) {
      for (auto j = 0; j < height; ++j) {
        auto length = BufferUtils::ReadUInt16(s, offset) - 2;
        RLEZerosCompression::DecodeInto(BufferUtils::ReadBytes(s, offset, length), data_, data_width * j);
      }
    } else {
      auto length = format_ == 2 ? BufferUtils::ReadUInt16(s, offset) - 2 : width;
      for (auto j = 0; j < height; ++j) {
        BufferUtils::ReadBytes(s, data_, data_width * j, offset, length);
      }
    }

    offset = start;
  }

  const Size& size() const override { return size_; }
  const Size& frame_size() const override { return frame_size_; }
  const Float2& offset() const override { return offset_; }
  std::vector<char>& data() override { return data_; }
  bool disable_export_padding() const override { return false; }

private:
  std::vector<char> data_;
  Size size_;
  Size frame_size_;
  Float2 offset_ = Float2::Zero;
  uint8_t format_ = 0;
  uint32_t file_offset_ = 0;
};

static void ParseFrames(const std::vector<char>& s, std::vector<ISpriteFramePtr>& frames) {
  size_t offset = 0;

  BufferUtils::ReadUInt16(s, offset);
  auto width = BufferUtils::ReadUInt16(s, offset);
  auto height = BufferUtils::ReadUInt16(s, offset);
  Size size = { width, height };
  auto frame_count = BufferUtils::ReadUInt16(s, offset);

  frames.reserve(frame_count);
  for (auto i = 0; i < frame_count; ++i) {
    frames.emplace_back(std::make_shared<ShpTSFrame>(s, offset, size));
  }
}

bool ShpTSLoader::TryParseSprite(const std::vector<char>&s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsShpTS(s)) {
    return false;
  }

  ParseFrames(s, frames);
  return true;
}

}
}
}
