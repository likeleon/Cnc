#include "cnc/stdafx.h"
#include "cnc/path.h"

namespace cnc {

std::string Path::GetDirectoryName(const std::string& path) {
  return std::tr2::sys::path(path).parent_path().string();
}

std::string Path::GetFileName(const std::string& path) {
  return std::tr2::sys::path(path).filename().string();
}

char Path::DirectorySeparator() {
  return std::tr2::sys::path::preferred_separator;
}

}