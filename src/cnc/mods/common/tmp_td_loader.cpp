#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/tmp_td_loader.h"
#include "cnc/buffer_utils.h"
#include "cnc/float2.h"
#include "cnc/size.h"

namespace cnc {
namespace mods {
namespace common {

static bool IsTmpTD(const std::vector<char>& s) {
  size_t offset = 0;
  offset += 16;
  auto a = BufferUtils::ReadUInt16(s, offset);
  auto b = BufferUtils::ReadUInt16(s, offset);
  return a == 0 && b == 0x0D1AFFFF;
}

class TmpTDFrame : public ISpriteFrame {
public:
  TmpTDFrame(std::vector<char>&& data, const Size& size)
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

static void ParseFrames(const std::vector<char>& s, std::vector<ISpriteFramePtr>& frames) {
  size_t offset = 0;
  auto width = BufferUtils::ReadUInt16(s, offset);
  auto height = BufferUtils::ReadUInt16(s, offset);
  Size size = { width, height };

  offset += 8;
  auto img_start = BufferUtils::ReadUInt32(s, offset);
  offset += 8;
  auto index_end = BufferUtils::ReadInt32(s, offset);
  auto index_start = BufferUtils::ReadInt32(s, offset);

  offset = index_start;
  auto count = index_end - index_start;
  frames.reserve(count);
  for (const auto& b : BufferUtils::ReadBytes(s, offset, count)) {
    if (b != 255) {
      offset = img_start + b * width * height;
      frames.emplace_back(std::make_shared<TmpTDFrame>(BufferUtils::ReadBytes(s, offset, width * height), size));
    } else {
      frames.emplace_back(std::make_shared<TmpTDFrame>(std::vector<char>(), size));
    }
  }
}

bool TmpTDLoader::TryParseSprite(const std::vector<char>& s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsTmpTD(s)) {
    return false;
  }

  ParseFrames(s, frames);
  return true;
}

}
}
}