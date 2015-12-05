#include "cnc/stdafx.h"
#include "cnc/folder.h"
#include "cnc/platform.h"
#include "cnc/package_entry.h"

namespace cnc {

Folder::Folder(const std::string& path, int32_t priority)
  : path_(path), priority_(priority) {
  if (!Platform::Exists(path_)) {
    Platform::CreateDir(path_);
  }
}

std::vector<uint32_t> Folder::ClassicHashes() const {
  std::vector<uint32_t> hashes;
  for (const auto& file : Platform::GetFiles(path_)) {
    std::string filename = Platform::GetFileName(file);
    hashes.emplace_back(PackageEntry::HashFilename(filename, PackageHashType::Classic));
  }
  return hashes;
}

std::vector<uint32_t> Folder::CrcHashes() const {
  return {};
}

}