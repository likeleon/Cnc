#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/shp_ts_loader.h"
#include "cnc/size.h"
#include "cnc/float2.h"
#include "cnc/stream.h"
#include "cnc/mods/common/rle_zeros_compression.h"

namespace cnc {
namespace mods {
namespace common {

static bool IsShpTS(StreamPtr s) {
  auto start = s->Position();
  
  if (s->ReadUInt16() != 0) {
    s->SetPosition(start);
    return false;
  }

  s->Seek(4, SeekOrigin::Current);
  auto image_count = s->ReadUInt16();
  if (s->Position() + 24 * image_count > s->Length()) {
    s->SetPosition(start);
    return false;
  }

  s->Seek(4, SeekOrigin::Current);
  uint16_t w = 0, h = 0, f = 0;
  uint8_t type = 0;
  do {
    w = s->ReadUInt16();
    h = s->ReadUInt16();
    type = s->ReadUInt8();
  } while (w == 0 && h == 0 && f++ < image_count);

  s->SetPosition(start);
  return type < 4;
}

class ShpTSFrame : public ISpriteFrame {
public:
  ShpTSFrame(StreamPtr s, const Size& frame_size) {
    auto x = s->ReadUInt16();
    auto y = s->ReadUInt16();
    auto width = s->ReadUInt16();
    auto height = s->ReadUInt16();

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

    format_ = s->ReadUInt8();
    s->Seek(11, SeekOrigin::Current);
    file_offset_ = s->ReadUInt32();

    if (file_offset_ == 0) {
      return;
    }

    auto start = s->Position();
    s->SetPosition(file_offset_);

    data_.resize(data_width * data_height);

    if (format_ == 3) {
      for (auto j = 0; j < height; ++j) {
        auto length = s->ReadUInt16() - 2;
        RLEZerosCompression::DecodeInto(s->ReadBytes(length), data_, data_width * j);
      }
    } else {
      auto length = format_ == 2 ? s->ReadUInt16() - 2 : width;
      for (auto j = 0; j < height; ++j) {
        s->ReadBytes(data_, data_width * j, length);
      }
    }

    s->SetPosition(start);
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

static std::vector<ISpriteFramePtr> ParseFrames(StreamPtr s) {
  auto start = s->Position();

  s->ReadUInt16();
  auto width = s->ReadUInt16();
  auto height = s->ReadUInt16();
  Size size = { width, height };
  auto frame_count = s->ReadUInt16();

  std::vector<ISpriteFramePtr> frames(frame_count);
  for (auto i = 0; i < frame_count; ++i) {
    frames[i] = std::make_shared<ShpTSFrame>(s, size);
  }
  
  s->SetPosition(start);
  return frames;
}

bool ShpTSLoader::TryParseSprite(StreamPtr s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsShpTS(s)) {
    return false;
  }

  frames = ParseFrames(s);
  return true;
}

}
}
}
