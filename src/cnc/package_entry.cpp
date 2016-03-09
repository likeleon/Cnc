#include "cnc/stdafx.h"
#include "cnc/package_entry.h"
#include "cnc/string_utils.h"
#include "cnc/error.h"
#include "cnc/stream.h"

namespace cnc {

const std::string PackageHashTypeTraits::pretty_name = "PackageHashType";

const EnumNamesType<PackageHashTypeTraits> PackageHashTypeTraits::names = {
  { PackageHashType::Classic, "Classic" },
  { PackageHashType::CRC32, "CRC32" },
};

PackageEntry::PackageEntry(StreamPtr s) {
  hash_ = s->ReadUInt32();
  offset_ = s->ReadUInt32();
  length_ = s->ReadUInt32();
}

uint32_t PackageEntry::HashFilename(const std::string& name, PackageHashType type) {
  if (type == PackageHashType::Classic) {
    auto n = StringUtils::ToUpper(name);
    if (n.length() % 4 != 0) {
      n = StringUtils::PadRight(n, n.length() + (4 - n.length() % 4), '\0');
    }

    std::stringstream ss(n);
    auto len = n.length() >> 2;
    uint32_t result = 0;
    while (len-- != 0) {
      uint32_t data = 0;
      ss.read((char*)&data, 4);
      result = ((result << 1) | (result >> 31)) + data;
    }
    return result;
  } else {
    throw Error(MSG("Unknown hash type"));
  }
}

uint32_t PackageEntry::hash() const {
  return hash_;
}

uint32_t PackageEntry::offset() const {
  return offset_;
}

uint32_t PackageEntry::length() const {
  return length_;
}

}