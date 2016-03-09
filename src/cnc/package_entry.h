#pragma once

#include "cnc/enum_info.h"
#include "cnc/stream_ptr.h"

namespace cnc {

enum class PackageHashType {
  Classic,
  CRC32
};

struct CNC_API PackageHashTypeTraits {
  using E = PackageHashType;
  static const std::string pretty_name;
  static const EnumNamesType<PackageHashTypeTraits> names;
};


class CNC_API PackageEntry {
public:
  explicit PackageEntry(StreamPtr s);

  static uint32_t HashFilename(const std::string& name, PackageHashType type);

  uint32_t hash() const;
  uint32_t offset() const;
  uint32_t length() const;

  static const int32_t Size = 12;

private:
  uint32_t hash_ = 0;
  uint32_t offset_ = 0;
  uint32_t length_ = 0;
};

}