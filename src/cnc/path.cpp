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

std::string Path::Combine(const std::vector<std::string>& paths) {
  auto path = std::accumulate(paths.begin(), paths.end(), std::tr2::sys::path{},
                              [](const std::tr2::sys::path& a, const std::string& b) { return a / b; });;
  return path.string();
}

}