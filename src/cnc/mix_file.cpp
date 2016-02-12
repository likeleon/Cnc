#include "cnc/stdafx.h"
#include "cnc/mix_file.h"
#include "cnc/buffer_utils.h"
#include "cnc/error.h"
#include "cnc/container_utils.h"

namespace cnc {

static std::vector<PackageEntry> ParseHeader(const std::vector<char>& s, size_t offset, size_t& header_end) {
  auto num_files = BufferUtils::ReadUInt16(s, offset);
  /*auto data_size = */BufferUtils::ReadUInt32(s, offset);

  std::vector<PackageEntry> items;
  for (auto i = 0; i < num_files; ++i) {
    items.emplace_back(s, offset);
  }
  header_end = offset + 6 + num_files * PackageEntry::Size;
  return items;
}

MixFile::MixFile(FileSystem& context, const std::string& filename, PackageHashType type, int32_t priority)
  : context_(context), filename_(filename), type_(type), priority_(priority) {
  s_ = context_.Open(filename);
  size_t offset = 0;
  
  auto is_cnc_mix = BufferUtils::ReadUInt16(s_, offset) != 0;

  auto is_encrypted = false;
  if (!is_cnc_mix) {
    is_encrypted = (BufferUtils::ReadUInt16(s_, offset) & 0x2) != 0;
  }

  std::vector<PackageEntry> entries;
  if (is_encrypted) {
    throw Error(MSG("TODO: Decrypt C&C mix format"));
  } else {
    entries = ParseHeader(s_, is_cnc_mix ? 0 : 4, data_start_);
  }

  std::ostringstream oss;
  oss << filename << " (" << (is_cnc_mix ? "C&C" : "RA/TS/RA2") << " format, Encrypted: " << is_encrypted << ", DataStart: " << data_start_ << ")";
  std::string debug_name = oss.str();
  auto key_selector = [](const PackageEntry& x) { return x.hash(); };
  auto log_value = [](const PackageEntry& x) {
    std::ostringstream oss;
    oss << "(offs=" << x.offset() << ", len=" << x.length() << ")";
    return oss.str();
  };
  index_ = ToMapWithConflictLog<PackageEntry, uint32_t>(entries, key_selector, debug_name, nullptr, log_value);
}

std::vector<char> MixFile::GetContent(const std::string& filename) const {
  auto hash = FindMatchingHash(filename);
  return hash ? GetContent(hash.value()) : std::vector<char>();
}

optional<uint32_t> MixFile::FindMatchingHash(const std::string& filename) const {
  auto hash = PackageEntry::HashFilename(filename, type_);
  if (index_.find(hash) != index_.end()) {
    return hash;
  }

  // TODO: raw hash given.

  return {};
}

std::vector<char> MixFile::GetContent(uint32_t hash) const {
  auto iter = index_.find(hash);
  if (iter == index_.end()) {
    return{};
  }

  //const auto& e = iter->second;
  
  // TODO: SegmentStreaam
  return{};
}

bool MixFile::Exists(const std::string& filename) const {
  return (bool)FindMatchingHash(filename);
}

std::vector<uint32_t> MixFile::ClassicHashes() const {
  std::vector<uint32_t> hashes;
  if (type_ == PackageHashType::Classic) {
    std::transform(index_.begin(), index_.end(), std::back_inserter(hashes), [](const auto& x) { return x.first; });
  }
  return hashes;
}

std::vector<uint32_t> MixFile::CrcHashes() const {
  std::vector<uint32_t> hashes;
  if (type_ == PackageHashType::CRC32) {
    std::transform(index_.begin(), index_.end(), std::back_inserter(hashes), [](const auto& x) { return x.first; });
  }
  return hashes;
}

int32_t MixFile::priority() const {
  return 1000 + priority_;
}

const std::string& MixFile::name() const {
  return filename_;
}

}