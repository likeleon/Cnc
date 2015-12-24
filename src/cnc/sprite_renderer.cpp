#include "cnc/stdafx.h"
#include "cnc/sprite_renderer.h"

namespace cnc {

SpriteRenderer::SpriteRenderer(Renderer* renderer, IShaderPtr shader)
  : renderer_(renderer), shader_(std::move(shader)) {
}

}