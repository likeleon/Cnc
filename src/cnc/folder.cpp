#include "cnc/stdafx.h"
#include "cnc/folder.h"
#include "cnc/package_entry.h"
#include "cnc/file.h"
#include "cnc/directory.h"
#include "cnc/path.h"

namespace cnc {

Folder::Folder(const std::string& path, int32_t priority)
  : path_(path), priority_(priority) {
  if (!File::Exists(path_)) {
    Directory::CreateDir(path_);
  }
}

StreamPtr Folder::GetContent(const std::string& filename) const {
  return File::OpenRead(Path::Combine({ path_, filename }));
}

bool Folder::Exists(const std::string& filename) const {
  return File::Exists(Path::Combine({ path_, filename }));
}

std::vector<uint32_t> Folder::ClassicHashes() const {
  std::vector<uint32_t> hashes;
  for (const auto& file : Directory::GetFiles(path_)) {
    std::string filename = Path::GetFileName(file);
    hashes.emplace_back(PackageEntry::HashFilename(filename, PackageHashType::Classic));
  }
  return hashes;
}

std::vector<uint32_t> Folder::CrcHashes() const {
  return {};
}

int32_t Folder::priority() const {
  return priority_;
}

const std::string& Folder::name() const {
  return path_;
}

}