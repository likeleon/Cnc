#include "cnc/stdafx.h"
#include "cnc/sdl2_graphics_device.h"
#include "cnc/log.h"
#include "cnc/game.h"
#include "cnc/settings.h"
#include "cnc/error_handler.h"
#include "cnc/error.h"
#include "cnc/shader.h"
#include "cnc/vertex.h"
#include "cnc/vertex_buffer.h"
#include "cnc/texture.h"

namespace cnc {

Sdl2GraphicsDevice::Sdl2GraphicsDevice(const Size& window_size, WindowMode window_mode)
  : window_size_(window_size) {
  std::cout << "Using SDL 2 with OpenGL renderer" << std::endl;;

  SDL_Init(SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_ALPHA_SIZE, 8);

  SDL_DisplayMode display;
  SDL_GetCurrentDisplayMode(0, &display);

  std::cout << "Desktop resolution: {" << display.w << "}x{" << display.h << "}" << std::endl;
  if (window_size_.width == 0 && window_size_.height == 0) {
    std::cout << "No custom resolution provided, using desktop resolution" << std::endl;
    window_size_ = Size(display.w, display.h);
  }

  std::cout << "Using resolution: {" << window_size_.width << "}x{" << window_size_.height << "}" << std::endl;

  window_ = SDL_CreateWindow("Cnc", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             window_size_.width, window_size_.height,
                             SDL_WindowFlags::SDL_WINDOW_OPENGL);

  if (Game::settings().game().lock_mouse_window) {
    GrabWindowMouseFocus();
  } else {
    ReleaseWindowMouseFocus();
  }

  if (window_mode == WindowMode::Fullscreen) {
    SDL_SetWindowFullscreen(window_, SDL_WindowFlags::SDL_WINDOW_FULLSCREEN);
  } else if (window_mode == WindowMode::PseudoFullscreen) {
    SDL_SetWindowFullscreen(window_, SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
  }

  context_ = SDL_GL_CreateContext(window_);
  if (context_ == NULL || SDL_GL_MakeCurrent(window_, context_) < 0) {
    auto text = std::string("Can not create OpenGL context. (Error: {") + SDL_GetError() + "}";
    throw Error(MSG(text));
  }

  ErrorHandler::CheckGlVersion();
  ErrorHandler::CheckGlError();

  if (SDL_GL_ExtensionSupported("GL_EXT_framebuffer_object") == SDL_bool::SDL_FALSE) {
    ErrorHandler::WriteGraphicsLog("Cnc requires the OpenGL extension GL_EXT_framebuffer_object.\n"
                                   "Please try updating your GPU driver to the lastest version provided by the manufacturer.");
    throw Error(MSG("Missing OpenGL extension GL_EXT_framebuffer_object. See graphics.log for details."));
  }

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    const char* err_str = reinterpret_cast<const char*>(glewGetErrorString(err));
    auto text = std::string("glewInit failed. (Error: {") + err_str + "}";
    throw Error(MSG(text));
  }

  glEnableVertexAttribArray(Shader::VertexPosAttributeIndex);
  ErrorHandler::CheckGlError();
  glEnableVertexAttribArray(Shader::TexCoordAttributeIndex);
  ErrorHandler::CheckGlError();

  SDL_SetModState(SDL_Keymod::KMOD_NONE);
}

Sdl2GraphicsDevice::~Sdl2GraphicsDevice() {
  if (context_ != nullptr) {
    SDL_GL_DeleteContext(context_);
  }
  
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
  }

  SDL_Quit();
}


std::unique_ptr<IVertexBuffer<Vertex>> Sdl2GraphicsDevice::CreateVertexBuffer(int32_t size) {
  VerifyThreadAffinity();
  return std::make_unique<VertexBuffer<Vertex>>(size);
}

IShaderPtr Sdl2GraphicsDevice::CreateShader(const std::string& name) {
  VerifyThreadAffinity();
  return std::make_unique<Shader>(name);
}

ITexturePtr Sdl2GraphicsDevice::CreateTexture() {
  VerifyThreadAffinity();
  return std::make_unique<Texture>();
}

void Sdl2GraphicsDevice::GrabWindowMouseFocus() {
  VerifyThreadAffinity();
  SDL_SetWindowGrab(window_, SDL_bool::SDL_TRUE);
}

void Sdl2GraphicsDevice::ReleaseWindowMouseFocus() {
  VerifyThreadAffinity();
  SDL_SetWindowGrab(window_, SDL_bool::SDL_FALSE);
}

static GLenum ModeFromPrimitiveType(PrimitiveType pt) {
  switch (pt) {
  case PrimitiveType::PointList:
    return GL_POINTS;
  case PrimitiveType::LineList:
    return GL_LINES;
  case PrimitiveType::TriangleList:
    return GL_TRIANGLES;
  default:
    throw Error(MSG(std::string("Unknown PrimitiveType: ") + std::to_string(static_cast<int32_t>(pt))));
  }
}

void Sdl2GraphicsDevice::DrawPrimitives(PrimitiveType pt, int32_t first_vertex, int32_t num_vertices) {
  VerifyThreadAffinity();
  glDrawArrays(ModeFromPrimitiveType(pt), first_vertex, num_vertices);
  ErrorHandler::CheckGlError();
}

void Sdl2GraphicsDevice::Clear() {
  VerifyThreadAffinity();
  glClearColor(0, 0, 0, 1);
  ErrorHandler::CheckGlError();
  glClear(GL_COLOR_BUFFER_BIT);
  ErrorHandler::CheckGlError();
}

void Sdl2GraphicsDevice::Present() {
  VerifyThreadAffinity();
  SDL_GL_SwapWindow(window_);
}

void Sdl2GraphicsDevice::PumpInput(IInputHandler& input_handler) {
  VerifyThreadAffinity();
  input_.PumpInput(input_handler);
}

void Sdl2GraphicsDevice::SetBlendMode(BlendMode mode) {
  VerifyThreadAffinity();
  glBlendEquation(GL_FUNC_ADD);
  ErrorHandler::CheckGlError();
 
  switch (mode) {
  case BlendMode::None:
    glDisable(GL_BLEND);
    break;

  case BlendMode::Alpha:
    glEnable(GL_BLEND);
    ErrorHandler::CheckGlError();
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    break;
  
  case BlendMode::Additive:
  case BlendMode::Subtractive:
    glEnable(GL_BLEND);
    ErrorHandler::CheckGlError();
    glBlendFunc(GL_ONE, GL_ONE);
    if (mode == BlendMode::Subtractive) {
      ErrorHandler::CheckGlError();
      glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
    }
    break;
  
  case BlendMode::Multiply:
    glEnable(GL_BLEND);
    ErrorHandler::CheckGlError();
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    ErrorHandler::CheckGlError();
    break;

  case BlendMode::Multiplicative:
    glEnable(GL_BLEND);
    ErrorHandler::CheckGlError();
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    break;

  case BlendMode::DoubleMultiplicative:
    glEnable(GL_BLEND);
    ErrorHandler::CheckGlError();
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
    break;
  }
  ErrorHandler::CheckGlError();
}

}