#pragma once

#include "cnc/thread_affine.h"
#include "cnc/ishader.h"

namespace cnc {

class Shader : public ThreadAffine, public IShader {
public:
  Shader(const std::string& name);

private:
  GLuint program_;
  std::unordered_map<std::string, GLint> samplers_;
};

}
