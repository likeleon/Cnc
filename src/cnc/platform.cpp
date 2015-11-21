#include "cnc/stdafx.h"
#include <shlobj.h>
#include "cnc/platform.h"
#include "cnc/fatal_exception.h"

namespace cnc {

using namespace std::tr2;

Path Platform::support_dir_;
Path Platform::game_dir_;

PlatformType Platform::GetCurrentPlatform() {
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

static Path GetPersonalDir() {
  LPWSTR ppszPath;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DONT_UNEXPAND, nullptr, &ppszPath);
  if (FAILED(hr)) {
    throw FatalException("Unable to get personal directory");
  }

  Path dir(ppszPath);
  CoTaskMemFree(ppszPath);
  return dir;
}

static Path GetSupportDirInternal() {
  // Use a local directory in the game root if it exists
  if (sys::exists("Support")) {
    return std::wstring(L"Support") + Path::preferred_separator;
  }

  Path dir = GetPersonalDir() / "Cnc";
  if (!sys::exists(dir)) {
    sys::create_directory(dir);
  }

  dir += Path::preferred_separator;
  return dir;
}

const Path& Platform::GetSupportDir() {
  if (support_dir_.empty()) {
    support_dir_ = GetSupportDirInternal();
  }
  return support_dir_;
}

const Path& Platform::GetGameDir() {
  if (game_dir_.empty()) {
    game_dir_ = sys::current_path();
  }
  return game_dir_;
}

Path Platform::ResolvePath(const std::wstring& path) {
  if (path.find(L"^") == 0) {
    return GetSupportDir() / path.substr(1);
  } else if (path.find(L"./") == 0 || path.find(L".\\") == 0) {
    return GetGameDir() / path.substr(2);
  } else {
    return path;
  }
}

Path Platform::ResolvePath(const std::vector<std::wstring>& paths) {
  auto path = std::accumulate(paths.begin(), paths.end(), Path{},
                              [](const Path& a, const std::wstring& b) { return a / b; });
  return ResolvePath(path.native());
}

}