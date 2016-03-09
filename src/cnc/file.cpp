#include "cnc/stdafx.h"
#include "cnc/file.h"
#include "cnc/error.h"
#include "cnc/file_stream.h"

namespace cnc {

bool File::Exists(const std::string& path) {
  return std::tr2::sys::exists(path);
}

StreamPtr File::OpenRead(const std::string& path) {
  return FileStream::CreateFrom(path);
}

std::string File::ReadAllText(const std::string& path) {
  std::ifstream ifs(path, std::ios::in | std::ios::ate);
  if (!ifs.is_open()) {
    throw Error(MSG("File not found: " + path));
  }

  size_t size = static_cast<size_t>(ifs.tellg());
  ifs.seekg(0, std::ios::beg);

  std::string str;
  str.resize(size);
  ifs.read(&str[0], size);
  return str;
}

}