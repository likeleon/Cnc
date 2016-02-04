#include "cnc/stdafx.h"
#include "cnc/error_handler.h"
#include "cnc/string_utils.h"
#include "cnc/log.h"
#include "cnc/error.h"

namespace cnc {

static std::string GlGetString(GLenum name) {
  return reinterpret_cast<const char*>(glGetString(name));
}

void ErrorHandler::CheckGlVersion() {
  std::string version_string = GlGetString(GL_VERSION);
  auto version = StringUtils::Contains(version_string, " ")
    ? StringUtils::Split(StringUtils::Split(version_string, ' ')[0], '.')
    : StringUtils::Split(version_string, '.');

  int32_t major = 0;
  if (version.size() > 0) {
    major = stoi(version[0]);
  }

  int32_t minor = 0;
  if (version.size() > 1) {
    minor = stoi(version[1]);
  }


  WriteGraphicsLog("Detected OpenGL version: " + version_string);
  if (major < 2) {
    WriteGraphicsLog("Cnc requires OpenGL version 2.0 or greater and detected " + version_string);
    throw Error(MSG("OpenGL Version ERror: See graphics.log for details."));
  }
}

void ErrorHandler::CheckGlError() {
  auto n = glGetError();
  if (n != GL_NO_ERROR) {
    WriteGraphicsLog("GL Error: " + std::to_string(n));
    throw Error(MSG("OpenGL Error: See graphics.log for details."));
  }
}
void ErrorHandler::WriteGraphicsLog(const std::string& message) {
  Log::Write("graphics", message);
  Log::Write("graphics", "");
  Log::Write("graphics", "OpenGL Information:");
  Log::Write("graphics", "Vendor: " + GlGetString(GL_VENDOR));
  if (StringUtils::Contains(GlGetString(GL_VENDOR), "Microsoft")) {
    std::string msg = "";
    msg += "Note:  The default driver provided by Microsoft does not include full OpenGL support.\n";
    msg += "Please install the latest drivers from your graphics card manufacturer's website.\n";
    Log::Write("graphics", msg);
  }

  Log::Write("graphics", "Renderer: " + GlGetString(GL_RENDERER));
  Log::Write("graphics", "GL Version: " + GlGetString(GL_VERSION));
  Log::Write("graphics", "Shader Version: " + GlGetString(GL_SHADING_LANGUAGE_VERSION));
  Log::Write("graphics", "Available extensions:");
  Log::Write("graphics", GlGetString(GL_EXTENSIONS));
}

}