#include "cnc/stdafx.h"
#include <shlobj.h>
#include "cnc/platform.h"
#include "cnc/fatal_exception.h"
#include "cnc/string.h"

namespace cnc {

using namespace std::tr2;
using fs_path = std::tr2::sys::path;

std::string Platform::support_dir_;
std::string Platform::game_dir_;

PlatformType Platform::CurrentPlatform() {
  return PlatformType::Windows;
}

std::ostream& operator<<(std::ostream& os, PlatformType platform) {
  switch (platform) {
  case PlatformType::Windows:
    os << "Windows";
    break;
  case PlatformType::Unknown:
    os << "Unknown";
    break;
  }
  return os;
}

static fs_path GetPersonalDir() {
  LPWSTR ppszPath;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DONT_UNEXPAND, nullptr, &ppszPath);
  if (FAILED(hr)) {
    throw FatalException("Unable to get personal directory");
  }

  std::wstring dir(ppszPath);
  CoTaskMemFree(ppszPath);
  return dir;
}

static fs_path GetSupportDirInternal() {
  // Use a local directory in the game root if it exists
  if (sys::exists("Support")) {
    return "Support" + std::tr2::sys::path::preferred_separator;
  }

  fs_path dir = GetPersonalDir() / "Cnc";
  if (!sys::exists(dir)) {
    sys::create_directory(dir);
  }

  dir += fs_path::preferred_separator;
  return dir;
}

const std::string& Platform::SupportDir() {
  if (support_dir_.empty()) {
    support_dir_ = GetSupportDirInternal().string();
  }
  return support_dir_;
}

const std::string& Platform::GameDir() {
  if (game_dir_.empty()) {
    game_dir_ = sys::current_path().string();
  }
  return game_dir_;
}

std::string Platform::ResolvePath(const std::string& p) {
  auto path = String::TrimEnd(p);
  if (path.find("^") == 0) {
    return (fs_path(SupportDir()) / path.substr(1)).string();
  } else if (path.find("./") == 0 || path.find(".\\") == 0) {
    return (fs_path(GameDir()) / path.substr(2)).string();
  } else {
    return path;
  }
}

std::string Platform::ResolvePath(const std::vector<std::string>& paths) {
  auto path = std::accumulate(paths.begin(), paths.end(), fs_path{},
                              [](const fs_path& a, const std::string& b) { return a / b; });
  return ResolvePath(path.string());
}

bool Platform::CreateDir(const std::string& path) {
  return std::tr2::sys::create_directory(path);
}

bool Platform::Exists(const std::string& path) {
  return std::tr2::sys::exists(path);
}

std::vector<std::string> Platform::GetFiles(const std::string& path) {
  std::vector<std::string> files;
  for (auto& p : std::tr2::sys::directory_iterator(path)) {
    if (std::tr2::sys::is_regular_file(p)) {
      files.emplace_back(p.path().string());
    }
  }
  return files;
}

std::string Platform::GetFileName(const std::string& path) {
  return fs_path(path).filename().string();
}

void Platform::Sleep(int64_t duration_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
}

}