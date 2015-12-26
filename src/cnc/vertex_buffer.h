#pragma once

#include "cnc/thread_affine.h"
#include "cnc/ivertex_buffer.h"
#include "cnc/error_handler.h"
#include "cnc/game.h"

namespace cnc {

template <typename T>
class VertexBuffer : public ThreadAffine, public IVertexBuffer<T> {
public:
  explicit VertexBuffer(int32_t size);
  virtual ~VertexBuffer();

  void Bind() override;
  void SetData(const std::vector<T>& vertices, int32_t length) override;
  void SetData(const std::vector<T>& vertices, int32_t start, int32_t length) override;

private:
  static const size_t VertexSize = sizeof(T);

  GLuint buffer_ = 0;
};

template <typename T>
VertexBuffer<T>::VertexBuffer(int32_t size) {
  glGenBuffers(1, &buffer_);
  ErrorHandler::CheckGlError();
  Bind();
  glBufferData(GL_ARRAY_BUFFER, VertexSize * size, NULL, GL_DYNAMIC_DRAW);
}

template <typename T>
VertexBuffer<T>::~VertexBuffer() {
  glDeleteBuffers(1, &buffer_);
}

template <typename T>
void VertexBuffer<T>::Bind() {
  VerifyThreadAffinity();
  glBindBuffer(GL_ARRAY_BUFFER, buffer_);
  ErrorHandler::CheckGlError();
  glVertexPointer(3, GL_FLOAT, VertexSize, NULL);
  ErrorHandler::CheckGlError();
  glTexCoordPointer(4, GL_FLOAT, VertexSize, (const void *)(12));
  ErrorHandler::CheckGlError();
}

template <typename T>
void VertexBuffer<T>::SetData(const std::vector<T>& vertices, int32_t length) {
  SetData(vertices, 0, length);

}

template <typename T>
void VertexBuffer<T>::SetData(const std::vector<T>& vertices, int32_t start, int32_t length) {
  Bind();
  glBufferSubData(GL_ARRAY_BUFFER, VertexSize * start, VertexSize * length, &vertices[0]);
  ErrorHandler::CheckGlError();
}

}
