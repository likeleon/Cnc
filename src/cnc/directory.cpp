#include "cnc/stdafx.h"
#include "cnc/directory.h"

namespace cnc {

bool Directory::Exists(const std::string& path) {
  return std::tr2::sys::exists(path) && std::tr2::sys::is_directory(path);
}

bool Directory::CreateDir(const std::string& path) {
  return std::tr2::sys::create_directories(path);
}

std::vector<std::string> Directory::GetFiles(const std::string& path) {
  std::vector<std::string> files;
  for (auto& p : std::tr2::sys::directory_iterator(path)) {
    if (std::tr2::sys::is_regular_file(p)) {
      files.emplace_back(p.path().string());
    }
  }
  return files;
}

std::vector<std::string> Directory::GetDirectories(const std::string& path) {
  std::vector<std::string> directories;
  for (auto& p : std::tr2::sys::directory_iterator(path)) {
    if (std::tr2::sys::is_directory(p)) {
      directories.emplace_back(p.path().string());
    }
  }
  return directories;
}

}