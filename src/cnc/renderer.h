#pragma once

#include "cnc/igraphics_device.h"
#include "cnc/sprite_renderer.h"
#include "cnc/ivertex_buffer.h"
#include "cnc/perf_history.h"
#include "cnc/size.h"
#include "cnc/point.h"
#include "cnc/sheet_builder.h"
#include "cnc/sprite_font.h"

namespace cnc {

struct GraphicSettings;
class IInputHandler;
class IBatchRenderer;
struct Vertex;
class Manifest;

class CNC_API Renderer {
public:
  explicit Renderer(const GraphicSettings& graphic_settings);
  ~Renderer();

  void InitializeFonts(const Manifest& m);

  void BeginFrame(const Point& scroll, float zoom);
  void SetViewportParms(const Point& scroll, float zoom);
  void EndFrame(IInputHandler& input_handler);

  void DrawBatch(const std::vector<Vertex>& vertices, int32_t num_vertices, PrimitiveType type);
  
  template <typename T>
  void DrawBatch(IVertexBuffer<T>& vertices, int32_t first_vertex, int32_t num_vertices, PrimitiveType type);

  void Flush();

  int32_t temp_buffer_size() const;
  const Size& Resolution() const;
  
  IGraphicsDevice& device();
  SpriteRenderer& rgba_sprite_renderer();
  const std::map<std::string, SpriteFontUniquePtr>& fonts() const;

  void SetCurrentBatchRenderer(IBatchRenderer* r);
  IBatchRenderer* current_batch_renderer() const;

private:
  IGraphicsDevicePtr device_;
  int32_t temp_buffer_size_ = 0;
  std::unique_ptr<SpriteRenderer> rgba_sprite_renderer_;
  IBatchRenderer* current_batch_renderer_ = nullptr;
  std::unique_ptr<IVertexBuffer<Vertex>> temp_buffer_;
  std::unique_ptr<SheetBuilder> font_sheet_builder_;
  std::map<std::string, SpriteFontUniquePtr> fonts_;
  Size last_resolution_;
  Point last_scroll_ = Point::Zero;
  float last_zoom_ = 0.0f;
};

template <typename T>
void Renderer::DrawBatch(IVertexBuffer<T>& vertices, int32_t first_vertex, int32_t num_vertices, PrimitiveType type) {
  vertices.Bind();
  device_->DrawPrimitives(type, first_vertex, num_vertices);
  PerfHistory::Increment("batches", 1);
}

}
