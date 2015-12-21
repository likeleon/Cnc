#include "cnc/stdafx.h"
#include "cnc/file.h"
#include "cnc/error.h"

namespace cnc {

std::string File::OpenRead(const std::string& path) {
  std::ifstream ifs(path, std::ios::binary | std::ios::in | std::ios::ate);
  if (!ifs.is_open()) {
    throw Error(MSG("File not found: " + path));
  }
  
  size_t size = static_cast<size_t>(ifs.tellg());
  ifs.seekg(0, std::ios::beg);

  std::vector<char> bytes(size);
  ifs.read(&bytes[0], size);
  return{ &bytes[0], size };
}

}