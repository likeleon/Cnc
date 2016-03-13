#include "cnc/stdafx.h"
#include "cnc/zip_file.h"
#include "cnc/file.h"
#include "cnc/error.h"
#include "cnc/memory_stream.h"
#include "cnc/package_entry.h"
#include "cnc/byte_stream_buffer.h"

namespace cnc {

ZipFile::ZipFile(FileSystem& context, const std::string& filename, int32_t priority)
  : context_(context), filename_(filename), priority_(priority) {
  stream_buf_ = std::make_unique<ByteStreamBuffer>(File::ReadAllBytes(filename));
  archive_ = ::ZipArchive::Create(new std::istream(stream_buf_.get()), true);
}

ZipFile::~ZipFile() {
  archive_ = nullptr;
  stream_buf_ = nullptr;
}

StreamPtr ZipFile::GetContent(const std::string& filename) const {
  auto entry = archive_->GetEntry(filename);
  if (entry == nullptr) {
    return nullptr;
  }
  
  if (entry->IsDirectory()) {
    return nullptr;
  }
 
  auto* stream = entry->GetDecompressionStream();
  if (stream == nullptr) {
    throw Error(MSG("Failed to get decompressed stream: " + entry->GetFullName()));
  }
  
  std::istreambuf_iterator<char> eos;
  std::vector<char> buffer(std::istreambuf_iterator<char>(*stream), eos);
  entry->CloseDecompressionStream();
  return std::make_unique<MemoryStream>(std::move(buffer));
}

bool ZipFile::Exists(const std::string& filename) const {
  return archive_->GetEntry(filename) != nullptr;
}

std::vector<uint32_t> ZipFile::ClassicHashes() const {
  std::vector<uint32_t> hashes;
  for (size_t i = 0; i < archive_->GetEntriesCount(); ++i) {
    auto entry = archive_->GetEntry(i);
    if (entry->IsDirectory()) {
      continue;
    }

    hashes.emplace_back(PackageEntry::HashFilename(entry->GetFullName(), PackageHashType::Classic));
  }
  return hashes;
}

std::vector<uint32_t> ZipFile::CrcHashes() const {
  return{};
}

int32_t ZipFile::priority() const {
  return 500 + priority_;
}

const std::string& ZipFile::name() const {
  return filename_;
}

}