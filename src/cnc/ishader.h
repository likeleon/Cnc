#pragma once

#include "cnc/itexture.h"

namespace cnc {

class IShader {
public:
  using RenderAction = std::function<void ()>;

  virtual ~IShader() {}

  virtual void SetVec(const std::string& name, float x, float y) = 0;
  virtual void SetTexture(const std::string& name, const ITexturePtr& texture) = 0;
  virtual void Render(const RenderAction& a) = 0;
};

using IShaderPtr = std::unique_ptr<IShader>;

}