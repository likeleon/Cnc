#pragma once

#include "cnc/stream.h"

namespace cnc {

class FileStream : public Stream {
public:
  static std::shared_ptr<FileStream> CreateFrom(const std::string& path);

private:
  FileStream(std::vector<char>&& bytes);
};

}
