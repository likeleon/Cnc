#pragma once

#include "cnc/ishader.h"

namespace cnc {

class Renderer;
class IShader;

class SpriteRenderer {
public:
  SpriteRenderer(Renderer* renderer, IShaderPtr shader);

private:
  Renderer* renderer_;
  IShaderPtr shader_;
};

}
