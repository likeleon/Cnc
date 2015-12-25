#pragma once

#include "cnc/thread_affine.h"
#include "cnc/ishader.h"
#include "cnc/itexture.h"

namespace cnc {

class Shader : public ThreadAffine, public IShader {
public:
  Shader(const std::string& name);

  void SetVec(const std::string& name, float x, float y) override;
  void SetTexture(const std::string& name, const ITexturePtr& t) override;
  void Render(const RenderAction& a) override;

private:
  GLuint program_;
  std::unordered_map<std::string, GLint> samplers_;
  std::unordered_map<GLint, ITexturePtr> textures_;
};

}
