#pragma once

namespace cnc {

enum class PackageHashType {
  Classic,
  CRC32
};

class PackageEntry {
public:
  static uint32_t HashFilename(const std::string& name, PackageHashType type);
};

}