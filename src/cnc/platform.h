#pragma once

namespace cnc {

enum class PlatformType {
  Unknown,
  Windows
};

std::ostream& operator<<(std::ostream& os, PlatformType platform);

class Platform {
public:
  static PlatformType CurrentPlatform();
  
  static const std::string& SupportDir();
  static const std::string& GameDir();
  
  static std::string ResolvePath(const std::string& path);
  static std::string ResolvePaths(const std::vector<std::string>& paths);

  static bool CreateDir(const std::string& path);
  static bool Exists(const std::string& path);
  static std::vector<std::string> GetFiles(const std::string& path);
  static std::vector<std::string> GetDirectories(const std::string& path);
  
  static std::string GetDirectoryName(const std::string& path);
  static std::string GetFileName(const std::string& path);

  static void Sleep(int64_t duration_ms);

private:
  static std::string support_dir_;
  static std::string game_dir_;
};

}