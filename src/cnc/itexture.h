#pragma once

namespace cnc {

struct Size;

enum class TextureScaleFilter {
  Nearest,
  Linear
};

class ITexture {
public:
  virtual ~ITexture() {}

  virtual void SetData(const std::string& colors, int32_t width, int32_t height) = 0;
  virtual std::string GetData() = 0;

  virtual const Size& size() const = 0;
  virtual TextureScaleFilter scale_filter() const = 0;
};

using ITexturePtr = std::shared_ptr<ITexture>;

}
