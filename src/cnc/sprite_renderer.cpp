#include "cnc/stdafx.h"
#include "cnc/sprite_renderer.h"
#include "cnc/sprite.h"
#include "cnc/renderer.h"
#include "cnc/shader.h"
#include "cnc/graphics_util.h"
#include "cnc/sheet.h"

namespace cnc {

SpriteRenderer::SpriteRenderer(Renderer* renderer, IShaderPtr shader)
  : renderer_(renderer), shader_(std::move(shader)) {
  vertices_.resize(renderer->temp_buffer_size(), { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
  render_action_ = [this]() { 
    renderer_->DrawBatch(vertices_, nv_, PrimitiveType::QuadList);
  };
}

void SpriteRenderer::DrawSprite(const Sprite& s, const Float2& location) {
  DrawSprite(s, location, 0, s.size);
}

void SpriteRenderer::SetViewportParams(const Size& screen, float zoom, const Point& scroll) {
  shader_->SetVec("Scroll", static_cast<float>(scroll.x), static_cast<float>(scroll.y));
  shader_->SetVec("r1", zoom * 2.0f / screen.width, -zoom * 2.0f / screen.height);
  shader_->SetVec("r2", -1, 1);
}

void SpriteRenderer::DrawSprite(const Sprite& s,
                                const Float2 location,
                                float palette_texture_index,
                                const Float2& size) {
  SetRenderStateForSprite(s);
  GraphicsUtil::FastCreateQuad(vertices_, location + s.fractional_offset * size, s, palette_texture_index, nv_, size);
  nv_ += 4;
}

void SpriteRenderer::SetRenderStateForSprite(const Sprite& s) {
  renderer_->SetCurrentBatchRenderer(this);

  if (s.blend_mode != current_blend_ || s.sheet != current_sheet_ || nv_ + 4 > renderer_->temp_buffer_size()) {
    Flush();
  }

  current_blend_ = s.blend_mode;
  current_sheet_ = s.sheet;
}

void SpriteRenderer::Flush() {
  if (nv_ <= 0) {
    return;
  }

  shader_->SetTexture("DiffuseTexture", current_sheet_->GetTexture());

  renderer_->device().SetBlendMode(current_blend_);
  shader_->Render(render_action_);
  renderer_->device().SetBlendMode(BlendMode::None);

  nv_ = 0;
  current_sheet_ = nullptr;
}

}