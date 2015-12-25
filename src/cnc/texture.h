#pragma once

#include "cnc/thread_affine.h"
#include "cnc/itexture.h"
#include "cnc/size.h"

namespace cnc {

class Texture : public ThreadAffine, public ITexture {
public:
  Texture();
  virtual ~Texture();

  void SetScaleFilter(TextureScaleFilter f);

  GLuint id() const;

  void SetData(const std::string & colors, int32_t width, int32_t height) override;
  std::string GetData() override;
  const Size & size() const override;
  TextureScaleFilter scale_filter() const override;

private:
  void PrepareTexture();

  GLuint texture_ = 0;
  Size size_;
  TextureScaleFilter scale_filter_ = TextureScaleFilter::Linear;
};

}
