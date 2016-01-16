#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/shp_td_loader.h"
#include "cnc/buffer_utils.h"

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
    ImageHeader(const std::vector<char>& stream, ShpTDSprite::Ptr reader);
    
    const Size & size() const override;
    const Size& frame_size() const override;
    const Float2& offset() const override;
    const std::vector<char>& data() const override;
    bool disable_export_padding() const override;
  
  private:
    ShpTDSprite::Ptr reader_;
    std::vector<char> data_;
  };

  explicit ShpTDSprite(const std::vector<char>& stream);
};

bool ShpTDLoader::TryParseSprite(const std::vector<char>& s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsShpTD(s)) {
    return false;
  }
  
  ShpTDSprite sprite(s);
  std::move(sprite.frames().begin(), sprite.frames().end(), std::back_inserter(frames));
  return false;
}

}
}
}
