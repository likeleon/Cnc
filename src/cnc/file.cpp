#include "cnc/stdafx.h"
#include "cnc/file.h"
#include "cnc/error.h"
#include "cnc/file_stream.h"

namespace cnc {

bool File::Exists(const std::string& path) {
  return std::tr2::sys::exists(path) && std::tr2::sys::is_regular_file(path);
}

StreamPtr File::OpenRead(const std::string& path) {
  return FileStream::CreateFrom(path);
}

std::vector<char> File::ReadAllBytes(const std::string& path) {
  std::ifstream ifs(path, std::ios::in | std::ios::ate | std::ios::binary);
  if (!ifs.is_open()) {
    throw Error(MSG("File not found: " + path));
  }

  size_t size = static_cast<size_t>(ifs.tellg());
  ifs.seekg(0, std::ios::beg);

  std::vector<char> result(size);
  ifs.read(&result[0], size);
  return result;
}

std::string File::ReadAllText(const std::string& path) {
  auto bytes = ReadAllBytes(path);
  return{ &bytes[0], bytes.size() };
}

}