#include "cnc/stdafx.h"
#include "cnc/shader.h"
#include "cnc/path.h"
#include "cnc/file.h"
#include "cnc/error_handler.h"
#include "cnc/log.h"
#include "cnc/error.h"
#include "cnc/texture.h"
#include "cnc/platform.h"

namespace cnc {

static int32_t CompileShaderObject(GLenum type, const std::string& name) {
  auto ext = (type == GL_VERTEX_SHADER) ? "vert" : "frag";
  auto filename = Path::Combine({ Platform::GameDir(), "glsl", name + "." + ext });
  auto code = File::ReadAllText(filename);

  auto shader = glCreateShader(type);
  ErrorHandler::CheckGlError();
  const char* code_c_str = code.c_str();
  glShaderSource(shader, 1, &code_c_str, NULL);
  ErrorHandler::CheckGlError();
  glCompileShader(shader);
  ErrorHandler::CheckGlError();
  GLint success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  ErrorHandler::CheckGlError();
  if (success == GL_FALSE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log;
    log.resize(length);
    glGetShaderInfoLog(shader, length, NULL, &log[0]);
    
    Log::Write("graphics", "GL Info Log:\n" + log);
    throw Error(MSG("Compile error in shader object '" + filename + "'"));
  }
  return shader;
}

Shader::Shader(const std::string& name) {
  auto vertex_shader = CompileShaderObject(GL_VERTEX_SHADER, name);
  auto fragment_shader = CompileShaderObject(GL_FRAGMENT_SHADER, name);

  // Assemble program
  program_ = glCreateProgram();
  ErrorHandler::CheckGlError();

  glBindAttribLocation(program_, VertexPosAttributeIndex, "aVertexPosition");
  ErrorHandler::CheckGlError();
  glBindAttribLocation(program_, TexCoordAttributeIndex, "aVertexTexCoord");
  ErrorHandler::CheckGlError();

  glAttachShader(program_, vertex_shader);
  ErrorHandler::CheckGlError();
  glAttachShader(program_, fragment_shader);
  ErrorHandler::CheckGlError();

  glLinkProgram(program_);
  ErrorHandler::CheckGlError();
  GLint success = GL_FALSE;
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  ErrorHandler::CheckGlError();
  if (success == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
    std::string log;
    log.resize(length);
    glGetProgramInfoLog(program_, length, NULL, &log[0]);

    Log::Write("graphics", "GL Info Log:\n" + log);
    throw Error(MSG("Link error in shader object '" + name + "'"));
  }

  glUseProgram(program_);
  ErrorHandler::CheckGlError();

  GLint num_uniforms = 0;
  glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &num_uniforms);
  ErrorHandler::CheckGlError();

  GLint next_tex_unit = 0;
  for (GLint i = 0; i < num_uniforms; ++i) {
    GLsizei length = 0, size = 0;
    GLenum type;
    std::string sampler;
    sampler.resize(128);
    glGetActiveUniform(program_, i, 128, &length, &size, &type, &sampler[0]);
    ErrorHandler::CheckGlError();

    if (type == GL_SAMPLER_2D) {
      samplers_.emplace(sampler.c_str(), next_tex_unit);

      auto loc = glGetUniformLocation(program_, sampler.c_str());
      ErrorHandler::CheckGlError();
      glUniform1i(loc, next_tex_unit);
      ErrorHandler::CheckGlError();

      ++next_tex_unit;
    }
  }
}

void Shader::SetVec(const std::string& name, float x, float y) {
  VerifyThreadAffinity();
  glUseProgram(program_);
  ErrorHandler::CheckGlError();
  auto param = glGetUniformLocation(program_, name.c_str());
  ErrorHandler::CheckGlError();
  glUniform2f(param, x, y);
  ErrorHandler::CheckGlError();
}

void Shader::SetTexture(const std::string& name, const ITexturePtr& t) {
  VerifyThreadAffinity();
  if (t == nullptr) {
    return;
  }

  auto iter = samplers_.find(name);
  if (iter != samplers_.end()) {
    textures_.insert_or_assign(iter->second, t);
  }
}

void Shader::Render(const RenderAction& a) {
  VerifyThreadAffinity();
  glUseProgram(program_);

  for (const auto& kvp : textures_) {
    glActiveTexture(GL_TEXTURE0 + kvp.first);
    auto* texture = static_cast<Texture*>(kvp.second.get());
    glBindTexture(GL_TEXTURE_2D, texture->id());
  }

  ErrorHandler::CheckGlError();
  a();
  ErrorHandler::CheckGlError();
}

}