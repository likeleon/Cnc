#include "cnc/stdafx.h"
#include "cnc/path.h"
#include "cnc/error.h"

namespace cnc {

std::string Path::GetDirectoryName(const std::string& path) {
  return std::tr2::sys::path(path).parent_path().string();
}

std::string Path::GetFileName(const std::string& path) {
  return std::tr2::sys::path(path).filename().string();
}

std::string Path::GetTempPath() {
  char s[MAX_PATH];
  ::GetTempPathA(MAX_PATH, s);
  return s;
}

char Path::DirectorySeparator() {
  return std::tr2::sys::path::preferred_separator;
}

std::string Path::Combine(const std::vector<std::string>& paths) {
  auto path = std::accumulate(paths.begin(), paths.end(), std::tr2::sys::path{},
                              [](const std::tr2::sys::path& a, const std::string& b) { return a / b; });;
  return path.string();
}

std::string Path::GetRandomFileName() {
  char s[MAX_PATH];
  auto ret = GetTempFileNameA("", "CNC", 0, s);
  if (ret == 0) {
    throw Error(MSG("GetTempFileNameA failed"));
  }
  return s + 1; // 's' starts with "\"
}

}