#include "cnc/stdafx.h"
#include "cnc/package_entry.h"
#include "cnc/string_utils.h"
#include "cnc/error.h"

namespace cnc {

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

}