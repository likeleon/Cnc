#include "cnc/stdafx.h"
#include "texture.h"
#include "error_handler.h"
#include "math_utils.h"
#include "error.h"

namespace cnc {

Texture::Texture() {
  glGenTextures(1, &texture_);
  ErrorHandler::CheckGlError();
}

Texture::~Texture() {
  glDeleteTextures(1, &texture_);
}

void Texture::SetData(const std::string & colors, int32_t width, int32_t height) {
  VerifyThreadAffinity();
  if (!IsPowerOf2(width) || !IsPowerOf2(height)) {
    std::ostringstream oss;
    oss << "Non-power-of-two array " << width << "x" << height;
    throw Error(MSG(oss.str()));
  }

  size_ = { width, height };
  
  PrepareTexture();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 
               0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<const void*>(&colors[0]));
  ErrorHandler::CheckGlError();
}

void Texture::PrepareTexture() {
  ErrorHandler::CheckGlError();
  glBindTexture(GL_TEXTURE_2D, texture_);
  ErrorHandler::CheckGlError();

  GLint filter = (scale_filter_ == TextureScaleFilter::Linear) ? GL_LINEAR : GL_NEAREST;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  ErrorHandler::CheckGlError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  ErrorHandler::CheckGlError();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  ErrorHandler::CheckGlError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  ErrorHandler::CheckGlError();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  ErrorHandler::CheckGlError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  ErrorHandler::CheckGlError();
}

std::string Texture::GetData() {
  VerifyThreadAffinity();
  std::string data;
  data.resize(4 * size_.width * size_.height);

  ErrorHandler::CheckGlError();
  glBindTexture(GL_TEXTURE_2D, texture_);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(&data[0]));
  ErrorHandler::CheckGlError();
  return data;
}

const Size& Texture::size() const {
  return size_;
}

TextureScaleFilter Texture::scale_filter() const {
  return TextureScaleFilter();
}

GLuint Texture::id() const {
  return texture_;
}

void Texture::SetScaleFilter(TextureScaleFilter f) {
  VerifyThreadAffinity();
  if (scale_filter_ == f) {
    return;
  }

  scale_filter_ = f;
  PrepareTexture();
}

}
