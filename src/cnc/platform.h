#pragma once

namespace cnc {

enum class PlatformType {
  Unknown,
  Windows
};

CNC_API std::ostream& operator<<(std::ostream& os, PlatformType platform);

class CNC_API Platform {
public:
  static PlatformType CurrentPlatform();
  
  static const std::string& SupportDir();
  static const std::string& GameDir();
  
  static std::string ResolvePath(const std::string& path);
  static std::string ResolvePaths(const std::vector<std::string>& paths);
  
  static void Sleep(int64_t duration_ms);

private:
  static std::string support_dir_;
  static std::string game_dir_;
};

}