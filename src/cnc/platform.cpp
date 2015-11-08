#include "cnc/stdafx.h"
#include <shlobj.h>
#include "cnc/platform.h"
#include "cnc/fatal_exception.h"

namespace cnc {

using namespace std::tr2;

sys::path Platform::support_dir_;

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

static sys::path GetPersonalDir() {
  LPWSTR ppszPath;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DONT_UNEXPAND, nullptr, &ppszPath);
  if (FAILED(hr)) {
    throw FatalException("Unable to get personal directory");
  }

  sys::path dir(ppszPath);
  CoTaskMemFree(ppszPath);
  return dir;
}

static sys::path GetSupportDirInternal() {
  // Use a local directory in the game root if it exists
  if (!sys::exists("Support")) {
    return std::wstring(L"Support") + sys::path::preferred_separator;
  }

  sys::path dir = GetPersonalDir() / "Cnc";
  if (!sys::exists(dir))
    sys::create_directory(dir);

  return dir.native() .append(sys::path::preferred_separator);
}

const sys::path& Platform::GetSupportDir() {
  if (support_dir_.empty()) {
    support_dir_ = GetSupportDirInternal();
  }
  return support_dir_;
}

}