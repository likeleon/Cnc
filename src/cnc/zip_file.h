#pragma once

#include <ZipArchive.h>
#include "cnc/ifolder.h"

namespace cnc {

class FileSystem;
class ByteStreamBuffer;

class ZipFile : public IFolder {
public:
  ZipFile(FileSystem& context, const std::string& filename, int32_t priority);
  ~ZipFile();

  StreamPtr GetContent(const std::string& filename) const override;
  bool Exists(const std::string& filename) const override;
  std::vector<uint32_t> ClassicHashes() const override;
  std::vector<uint32_t> CrcHashes() const override;
  int32_t priority() const override;
  const std::string& name() const override;

private:
  FileSystem& context_;
  std::string filename_;
  int32_t priority_;
  std::unique_ptr<ByteStreamBuffer> stream_buf_;
  ZipArchive::Ptr archive_;

};

}
