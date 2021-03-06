#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/shp_td_loader.h"
#include "cnc/size.h"
#include "cnc/float2.h"
#include "cnc/error.h"
#include "cnc/stream.h"
#include "cnc/mods/common/lcw_compression.h"

namespace cnc {
namespace mods {
namespace common {

static bool IsShpTD(StreamPtr s) {
  auto start = s->Position();

  auto image_count = s->ReadUInt16();
  if (image_count == 0) {
    s->SetPosition(start);
    return false;
  }

  size_t final_offset = 14 + 8 * image_count;
  if (final_offset > s->Length()) {
    s->SetPosition(start);
    return false;
  }

  s->SetPosition(final_offset);
  auto eof = s->ReadUInt32();
  if (eof != s->Length()) {
    s->SetPosition(start);
    return false;
  }

  s->SetPosition(start + 17);
  auto b = s->ReadUInt8();

  s->SetPosition(start);
  return b == 0x20 || b == 0x40 || b == 0x80;
}

class ShpTDSprite : public std::enable_shared_from_this<ShpTDSprite> {
public:
  enum class Format {
    XORPrev = 0x20,
    XORLCW = 0x40,
    LCW = 0x80
  };

  using Ptr = std::shared_ptr<ShpTDSprite>;

  static Ptr LoadFrom(StreamPtr stream);

  class ImageHeader : public ISpriteFrame {
  public:
    ImageHeader(StreamPtr stream, ShpTDSprite::Ptr reader);

    const Size& size() const override { return reader_->size(); }
    const Size& frame_size() const override { return reader_->size(); }
    const Float2& offset() const override { return Float2::Zero; }
    std::vector<char>& data() override { return data_; }
    bool disable_export_padding() const override { return false; }

    uint32_t file_offset() const { return file_offset_; }
    Format format() const { return format_; }
    uint32_t ref_offset() const { return ref_offset_; }
    Format ref_format() const { return ref_format_; }
    const std::shared_ptr<ImageHeader>& ref_image() const { return ref_image_; }
    void set_ref_image(const std::shared_ptr<ImageHeader>& ref_image) { ref_image_ = ref_image; }

  private:
    ShpTDSprite::Ptr reader_;
    std::vector<char> data_;
    uint32_t file_offset_;
    Format format_;
    uint32_t ref_offset_;
    Format ref_format_;
    std::shared_ptr<ImageHeader> ref_image_;
  };

  const Size& size() const { return size_; }
  std::vector<ISpriteFramePtr>& frames() { return frames_; }

private:
  void LoadFrames(StreamPtr stream);
  void Decompress(const std::shared_ptr<ImageHeader>& h);

  int32_t recurse_depth_ = 0;
  int32_t image_count_ = 0;
  Size size_;
  std::vector<ISpriteFramePtr> frames_;
  size_t shp_bytes_file_offset_ = 0;
  std::vector<char> shp_bytes_;
};

ShpTDSprite::ImageHeader::ImageHeader(StreamPtr stream, ShpTDSprite::Ptr reader)
  : reader_(reader) {
  auto data = stream->ReadUInt32();
  file_offset_ = data & 0xffffff;
  format_ = static_cast<Format>(data >> 24);

  ref_offset_ = stream->ReadUInt16();
  ref_format_ = static_cast<Format>(stream->ReadUInt16());
}

ShpTDSprite::Ptr ShpTDSprite::LoadFrom(StreamPtr stream) {
  auto sprite = std::make_shared<ShpTDSprite>();
  sprite->LoadFrames(stream);
  return sprite;
}

void ShpTDSprite::LoadFrames(StreamPtr stream) {
  image_count_ = stream->ReadUInt16();
  stream->Seek(4, SeekOrigin::Current);
  auto width = stream->ReadUInt16();
  auto height = stream->ReadUInt16();
  size_ = { width, height };

  stream->Seek(4, SeekOrigin::Current);
  std::vector<std::shared_ptr<ImageHeader>> headers(image_count_);
  for (size_t i = 0; i < headers.size(); ++i) {
    headers[i] = std::make_shared<ImageHeader>(stream, shared_from_this());
    frames_.emplace_back(headers[i]);
  }

  stream->Seek(16, SeekOrigin::Current);

  std::map<uint32_t, std::shared_ptr<ImageHeader>> offsets;
  for (const auto& header : headers) {
    offsets.emplace(header->file_offset(), header);
  }
  for (auto i = 0; i < image_count_; ++i) {
    auto h = headers[i];
    if (h->format() == Format::XORPrev) {
      h->set_ref_image(headers[i - 1]);
    } else if (h->format() == Format::XORLCW && offsets.find(h->ref_offset()) == offsets.end()) {
      std::ostringstream oss;
      oss << "Reference doesn't point to image data " << h->file_offset() << "->" << h->ref_offset();
      throw Error(MSG(oss.str()));
    }
  }

  shp_bytes_file_offset_ = stream->Position();
  shp_bytes_ = stream->ReadBytes(stream->Length() - stream->Position());

  for (auto& h : headers) {
    Decompress(h);
  }
}

void ShpTDSprite::Decompress(const std::shared_ptr<ShpTDSprite::ImageHeader>& h) {
  if (h->size().width == 0 || h->size().height == 0) {
    return;
  }

  if (recurse_depth_ > image_count_) {
    throw Error(MSG("Format20/40 headers contain infinite loop"));
  }

  switch (h->format()) {
  case Format::XORPrev:
  case Format::XORLCW:
    if (h->ref_image()->data().empty()) {
      ++recurse_depth_;
      Decompress(h->ref_image());
      --recurse_depth_;
    }

    std::copy(h->ref_image()->data().begin(), h->ref_image()->data().end(), std::back_inserter(h->data()));
    throw Error(MSG("TODO: Decompress data format 'Format40'"));
    break;

  case Format::LCW: {
    std::vector<char> image_bytes(size_.width * size_.height);
    LCWCompression::DecodeInto(shp_bytes_, image_bytes, h->file_offset() - shp_bytes_file_offset_);
    h->data() = std::move(image_bytes);
    break;
    }
  default:
    throw Error(MSG("Invalid data format"));
  }
}

bool ShpTDLoader::TryParseSprite(StreamPtr s, std::vector<ISpriteFramePtr>& frames) {
  if (!IsShpTD(s)) {
    return false;
  }

  auto sprite = ShpTDSprite::LoadFrom(s);
  std::copy(sprite->frames().begin(), sprite->frames().end(), std::back_inserter(frames));
  return true;
}

}
}
}
