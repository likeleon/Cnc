#include "cnc/stdafx.h"
#include "cnc/renderer.h"
#include "cnc/settings.h"
#include "cnc/device_factory.h"
#include "cnc/sprite_renderer.h"
#include "cnc/manifest.h"
#include "cnc/platform.h"
#include "cnc/perf_timer.h"

namespace cnc {

static Size GetResolution(const GraphicSettings& graphics_settings) {
  return (graphics_settings.mode == WindowMode::Windowed)
    ? graphics_settings.windowed_size
    : graphics_settings.fullscreen_size;
}

Renderer::Renderer(const GraphicSettings& graphic_settings) {
  auto resolution = GetResolution(graphic_settings);
  auto renderer_name = graphic_settings.renderer;

  device_ = DeviceFactory::CreateGraphics(resolution, graphic_settings.mode);

  temp_buffer_size_ = graphic_settings.batch_size;

  rgba_sprite_renderer_ = std::make_unique<SpriteRenderer>(this, device_->CreateShader("rgba"));

  temp_buffer_ = device_->CreateVertexBuffer(temp_buffer_size_);
}

void Renderer::InitializeFonts(const Manifest& m) {
  PERF_TIMER("SpriteFonts", {
    TTF_Init();
    font_sheet_builder_ = std::make_unique<SheetBuilder>(SheetType::BGRA);
    fonts_.clear();
    for (const auto& f : m.fonts()) {
      auto font = std::make_unique<SpriteFont>(Platform::ResolvePath(f.second.first), f.second.second, *font_sheet_builder_);
      fonts_.emplace(f.first, std::move(font));
    }
    TTF_Quit();
  });
}

void Renderer::BeginFrame(const Point& scroll, float zoom) {
  device_->Clear();
  SetViewportParms(scroll, zoom);
}

void Renderer::SetViewportParms(const Point& scroll, float zoom) {
  bool resolution_changed = last_resolution_ != Resolution();
  if (resolution_changed) {
    last_resolution_ = Resolution();
    rgba_sprite_renderer_->SetViewportParams(Resolution(), 1.0f, Point::Zero);
  }

  if (resolution_changed || last_scroll_ != scroll || last_zoom_ != zoom) {
    last_scroll_ = scroll;
    last_zoom_ = zoom;
  }
}

void Renderer::EndFrame(IInputHandler& input_handler) {
  Flush();
  device_->PumpInput(input_handler);
  device_->Present();
}

void Renderer::DrawBatch(const std::vector<Vertex>& vertices, int32_t num_vertices, PrimitiveType type) {
  temp_buffer_->SetData(vertices, num_vertices);
  DrawBatch(*temp_buffer_, 0, num_vertices, type);
}

int32_t Renderer::temp_buffer_size() const {
  return temp_buffer_size_;
}

const Size& Renderer::Resolution() const {
  return device_->window_size();
}

IGraphicsDevice& Renderer::device() {
  return *device_;
}

SpriteRenderer& Renderer::rgba_sprite_renderer() {
  return *rgba_sprite_renderer_;
}

void Renderer::SetCurrentBatchRenderer(IBatchRenderer* r) {
  if (current_batch_renderer_ == r) {
    return;
  }
  if (current_batch_renderer_ != nullptr) {
    current_batch_renderer_->Flush();
  }
  current_batch_renderer_ = r;
}

IBatchRenderer* Renderer::current_batch_renderer() const {
  return current_batch_renderer_;
}

void Renderer::Flush() {
  SetCurrentBatchRenderer(nullptr);
}

}