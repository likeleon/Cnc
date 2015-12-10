#include "cnc/stdafx.h"
#include "cnc/renderer.h"
#include "cnc/settings.h"
#include "cnc/device_factory.h"

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
}

void Renderer::BeginFrame() {
  device_->Clear();
}

void Renderer::EndFrame(IInputHandler& input_handler) {
  device_->PumpInput(input_handler);
  device_->Present();
}

}