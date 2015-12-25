#pragma once

namespace cnc {

template <typename T>
class IVertexBuffer {
public:
  virtual ~IVertexBuffer() {}

  virtual void Bind() = 0;
  virtual void SetData(const std::vector<T>& vertices, int32_t length) = 0;
  virtual void SetData(const std::vector<T>& vertices, int32_t start, int32_t length) = 0;
};

}
