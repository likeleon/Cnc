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
  
  static const std::string& GetSupportDir();
  static const std::string& GetGameDir();
  
  static std::string ResolvePath(const std::string& path);
  static std::string ResolvePath(const std::vector<std::string>& paths);

  static bool CreateDir(const std::string& path);
  static bool Exists(const std::string& path);

private:
  static std::string support_dir_;
  static std::string game_dir_;
};

}