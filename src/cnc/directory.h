#pragma once

namespace cnc {

class CNC_API Directory {
public:
  Directory() = delete;

  static bool CreateDir(const std::string& path);

  static std::vector<std::string> GetFiles(const std::string& path);
  static std::vector<std::string> GetDirectories(const std::string& path);
};

}
