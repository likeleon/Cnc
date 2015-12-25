#pragma once

#include "cnc/enum_info.h"
#include "cnc/ishader.h"
#include "cnc/blend_mode.h"
#include "cnc/itexture.h"

namespace cnc {

class IInputHandler;
struct Size;
struct Vertex;
template <typename T>
class IVertexBuffer;

enum class PrimitiveType {
  PointList,
  LineList,
  TriangleList,
  QuadList
};

enum class WindowMode {
  Windowed,
  Fullscreen,
  PseudoFullscreen
};

template <>
struct CNC_API EnumInfoTraits<WindowMode> {
  static const std::string pretty_name;
  static const EnumInfo<WindowMode>::NamesType names;
};

class CNC_API IGraphicsDevice {
public:
  virtual ~IGraphicsDevice() {}

  virtual std::unique_ptr<IVertexBuffer<Vertex>> CreateVertexBuffer(int32_t size) = 0;
  virtual ITexturePtr CreateTexture() = 0;
  virtual IShaderPtr CreateShader(const std::string& name) = 0;

  virtual const Size& window_size() const = 0;

  virtual void Clear() = 0;
  virtual void Present() = 0;
  virtual void PumpInput(IInputHandler& input_handler) = 0;
  virtual void DrawPrimitives(PrimitiveType pt, int32_t first_vertex, int32_t num_vertices) = 0;

  virtual void SetBlendMode(BlendMode mode) = 0;
};

using IGraphicsDevicePtr = std::unique_ptr<IGraphicsDevice>;

}
