#include "cnc/stdafx.h"
#include "cnc/file_stream.h"
#include "cnc/error.h"

namespace cnc {

std::shared_ptr<FileStream> FileStream::CreateFrom(const std::string& path) {
  std::ifstream ifs(path, std::ios::binary | std::ios::in | std::ios::ate);
  if (!ifs.is_open()) {
    throw Error(MSG("File not found: " + path));
  }
  ifs.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);

  size_t size = static_cast<size_t>(ifs.tellg());
  ifs.seekg(0, std::ios::beg);

  std::vector<char> bytes(size);
  ifs.read(&bytes[0], size);
  return std::shared_ptr<FileStream>(new FileStream(std::move(bytes)));
}

FileStream::FileStream(std::vector<char>&& bytes)
  : Stream(std::move(bytes)) {
}

}