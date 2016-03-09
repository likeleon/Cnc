#pragma once

#include "cnc/file_system.h"

namespace cnc {

class FileSystem;

class MixFile : public IFolder {
public:
  MixFile(FileSystem& context, const std::string& filename, PackageHashType type, int32_t priority);

  StreamPtr GetContent(const std::string& filename) const override;
  bool Exists(const std::string& filename) const override;
  std::vector<uint32_t> ClassicHashes() const override;
  std::vector<uint32_t> CrcHashes() const override;
  int32_t priority() const override;
  const std::string& name() const override;

private:
  optional<uint32_t> FindMatchingHash(const std::string& filename) const;
  StreamPtr GetContent(uint32_t hash) const;
  
  std::map<uint32_t, PackageEntry> index_;
  size_t data_start_ = 0U;
  FileSystem& context_;
  std::string filename_;
  PackageHashType type_;
  int32_t priority_;
  StreamPtr s_;
};

}
