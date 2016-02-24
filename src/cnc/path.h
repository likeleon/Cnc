#pragma once

#ifdef GetTempPath
#undef GetTempPath
#endif

namespace cnc {

class CNC_API Path {
public:
  Path() = delete;

  static std::string GetDirectoryName(const std::string& path);
  static std::string GetFileName(const std::string& path);
  static std::string GetTempPath();
  static std::string GetRandomFileName();
  
  static char DirectorySeparator();
  
  static std::string Combine(const std::vector<std::string>& paths);
};

}
