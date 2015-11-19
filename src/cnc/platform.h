#pragma once

namespace cnc {

enum class PlatformType {
  Unknown,
  Windows
};

std::ostream& operator<<(std::ostream& os, PlatformType platform);

class Platform {
public:
  using Path = std::tr2::sys::path;

  static PlatformType GetCurrentPlatform();
  
  static const Path& GetSupportDir();
  static const Path& GetGameDir();
  
  static Path ResolvePath(const std::wstring& path);
  static Path ResolvePath(const std::vector<std::wstring>& paths);

private:
  static Path support_dir_;
  static Path game_dir_;
};

}