#include "cnc/stdafx.h"
#include <shlobj.h>
#include "cnc/platform.h"
#include "cnc/error.h"
#include "cnc/string_utils.h"
#include "cnc/path.h"

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
    throw Error(MSG("Unable to get personal directory"));
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
  auto path = StringUtils::TrimEnd(p);
  if (path.find("^") == 0) {
    return (fs_path(SupportDir()) / path.substr(1)).string();
  } else if (path.find("./") == 0 || path.find(".\\") == 0) {
    return (fs_path(GameDir()) / path.substr(2)).string();
  } else {
    return path;
  }
}

std::string Platform::ResolvePaths(const std::vector<std::string>& paths) {
  return ResolvePath(Path::Combine(paths));
}

void Platform::Sleep(int64_t duration_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
}

}