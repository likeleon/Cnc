#pragma once

namespace cnc {

enum class PlatformType {
  Unknown,
  Windows
};

std::ostream& operator<<(std::ostream& os, PlatformType platform);

class Platform {
public:
  static PlatformType GetCurrentPlatform();
  static const std::tr2::sys::path& GetSupportDir();

private:
  static std::tr2::sys::path support_dir_;
};

}