#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/shp_td_loader.h"
#include "cnc/buffer_utils.h"
#include "cnc/size.h"

namespace cnc {
namespace mods {
namespace common {

static bool IsShpTD(const std::vector<char>& s) {
  auto length = s.size();
  size_t offset = 0;

  auto image_count = BufferUtils::ReadUInt16(s, offset);
  if (image_count == 0) {
    return false;
  }

  size_t final_offset = 14 + 8 * image_count;
  if (final_offset > length) {
    return false;
  }

  offset = final_offset;
  auto eof = BufferUtils::ReadUInt32(s, offset);
  if (eof != length) {
    return false;
  }

  offset = 17;
  auto b = BufferUtils::ReadUInt8(s, offset);

  return b == 0x20 || b == 0x40 || b == 0x80;
}

class ShpTDSprite {
public:
  enum class Format {
    XORPrev = 0x20,
    XORLCW = 0x40,
    LCW = 0x80
  };

  using Ptr = std::shared_ptr<ShpTDSprite>;

  class ImageHeader : public ISpriteFrame {
  public:
    ImageHeader(const std::vector<char>& stream, size_t offset, ShpTDSprite::Ptr reader);

    const Size & size() const override { return reader_->size(); }
    const Size& frame_size() const override { return reader_->size(); }
    const Float2& offset() const override { return Float2::Zero; }
    const std::vector<char>& data() const override { return data_; }
    bool disable_export_padding() const override { return false; }

  private:
    ShpTDSprite::Ptr reader_;
    std::vector<char> data_;
    uint32_t file_offset_;
    Format format_;
    uint32_t ref_offset_;
    Format ref_format_;
  };

  explicit ShpTDSprite(const std::vector<char>& stream);
  
  std::vector<ISpriteFramePtr>& frames() { return frames_; }

private:
  int32_t image_count_;
  Size size_;
  std::vector<ISpriteFramePtr> frames_;
};

ShpTDSprite::ImageHeader::ImageHeader(const std::vector<char>& stream, size_t offset, ShpTDSprite::Ptr reader)
  : reader_(reader) {
  auto data = BufferUtils::ReadUInt32(stream, offset);
  file_offset_ = data & 0xffffff;
  format_ = static_cast<Format>(data >> 24);

  ref_offset_ = BufferUtils::ReadUInt16(stream, offset);
  ref_format_ = static_cast<Format>(BufferUtils::ReadUInt16(stream, offset));
}

ShpTDSprite::ShpTDSprite(const std::vector<char>& stream) {
  size_t offset = 0;
  image_count_ = BufferUtils::ReadUInt16(stream, offset);
  offset += 4;
  auto width = BufferUtils::ReadUInt16(stream, offset);
  auto height = BufferUtils::ReadUInt16(stream, offset);
  size_ = { width, height };

  offset += 4;
  for (auto i = 0; i < image_count_; ++i) {
    frames_.emplace_back(std::make_shared<ImageHeader>(stream, offset, this));
  }

  offset += 16;
}

bool ShpTDLoader::TryParseSprite(const std::vector<char>& s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsShpTD(s)) {
    return false;
  }

  ShpTDSprite sprite(s);
  std::copy(sprite.frames().begin(), sprite.frames().end(), std::back_inserter(frames));
  return true;
}

}
}
}
