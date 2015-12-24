#include "cnc/stdafx.h"
#include "cnc/renderer.h"
#include "cnc/settings.h"
#include "cnc/device_factory.h"
#include "cnc/sprite_renderer.h"

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

  rgba_sprite_renderer_ = std::make_unique<SpriteRenderer>(this, device_->CreateShader("rgba"));
}

void Renderer::BeginFrame() {
  device_->Clear();
}

void Renderer::EndFrame(IInputHandler& input_handler) {
  device_->PumpInput(input_handler);
  device_->Present();
}

const Size& Renderer::Resolution() const {
  return device_->window_size();
}

SpriteRenderer& Renderer::rgba_sprite_renderer() {
  return *rgba_sprite_renderer_;
}

}