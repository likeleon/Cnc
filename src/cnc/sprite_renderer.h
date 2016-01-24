#pragma once

#include "cnc/ishader.h"
#include "cnc/vertex.h"
#include "cnc/ibatch_renderer.h"
#include "cnc/blend_mode.h"
#include "cnc/sheet_ptr.h"
#include "cnc/ishader.h"
#include "cnc/point.h"

namespace cnc {

class Renderer;
struct Sprite;
struct Float2;

class SpriteRenderer : public IBatchRenderer {
public:
  SpriteRenderer(Renderer* renderer, IShaderPtr shader);

  void DrawSprite(const Sprite& s, const Float2& location);
  void SetViewportParams(const Size& screen, float zoom, const Point& scroll);
  void SetPalette(const ITexturePtr& texture);

  void Flush() override;

private:
  void DrawSprite(const Sprite& s, 
                  const Float2 location, 
                  float palette_texture_index, 
                  const Float2& size);
  void SetRenderStateForSprite(const Sprite& s);

  Renderer* renderer_;
  IShaderPtr shader_;
  std::vector<Vertex> vertices_;
  SheetPtr current_sheet_;
  BlendMode current_blend_ = BlendMode::Alpha;
  int32_t nv_ = 0;
  IShader::RenderAction render_action_;
};

}
