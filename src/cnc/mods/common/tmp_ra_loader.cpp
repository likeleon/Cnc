#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/tmp_ra_loader.h"
#include "cnc/stream.h"
#include "cnc/size.h"
#include "cnc/float2.h"

namespace cnc {
namespace mods {
namespace common {

static bool IsTmpRA(StreamPtr s) {
  auto start = s->Position();

  s->Seek(20, SeekOrigin::Current);
  auto a = s->ReadUInt32();
  s->Seek(2, SeekOrigin::Current);
  auto b = s->ReadUInt16();
  
  s->SetPosition(start);
  return a == 0 && b == 0x2c73;
}
class TmpRAFrame : public ISpriteFrame {
public:
  TmpRAFrame(std::vector<char>&& data, const Size& size)
    : frame_size_(size), data_(std::move(data)) {
    if (!data_.empty()) {
      size_ = size;
    }
  }

  const Size& size() const override { return size_; }
  const Size& frame_size() const override { return frame_size_; }
  const Float2& offset() const override { return Float2::Zero; }
  std::vector<char>& data() override { return data_; }
  bool disable_export_padding() const override { return false; }

private:
  std::vector<char> data_;
  Size size_;
  Size frame_size_;
};

static std::vector<ISpriteFramePtr> ParseFrames(StreamPtr s) {
  auto start = s->Position();
  auto width = s->ReadUInt16();
  auto height = s->ReadUInt16();
  Size size = { width, height };

  s->Seek(12, SeekOrigin::Current);
  auto img_start = s->ReadUInt32();
  s->Seek(8, SeekOrigin::Current);
  auto index_end = s->ReadInt32();
  s->Seek(4, SeekOrigin::Current);
  auto index_start = s->ReadInt32();

  s->SetPosition(index_start);
  auto count = index_end - index_start;
  std::vector<ISpriteFramePtr> tiles(count);
  
  auto tiles_index = 0;
  for (const auto& b : s->ReadBytes(count)) {
    if (b != 255) {
      s->SetPosition(img_start + b * width * height);
      tiles[tiles_index++] = std::make_shared<TmpRAFrame>(s->ReadBytes(width * height), size);
    } else {
      tiles[tiles_index++] = std::make_shared<TmpRAFrame>(std::vector<char>(), size);
    }
  }

  s->SetPosition(start);
  return tiles;
}

bool TmpRALoader::TryParseSprite(StreamPtr s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsTmpRA(s)) {
    return false;
  }

  frames = ParseFrames(s);
  return true;
}

}
}
}
