#pragma once

#include "cnc/memory_stream.h"

namespace cnc {

class FileStream : public MemoryStream {
public:
  static std::shared_ptr<FileStream> CreateFrom(const std::string& path);

  std::string path() const;

private:
  FileStream(std::vector<char>&& bytes);

  std::string path_;
};

}
